# Unit Testing — JUnit + Mockito (nakli supplier, asli cook)

> Asli test: usercrud `UserServiceTest` (4 test, sab PASS, JDK 17 pe chalaya 25-Nov).
> Laptop ka default JDK 27 hai, us pe Lombok compile pe phat-ta; project JDK 17 se chalao.

---

## Koi poochhe "test kaise karte ho" to aise bolo

Unit test me jis class ko test kar raha hoon wo ASLI rakhta hoon (`@InjectMocks`), aur uski dependency jaise repository
NAKLI (`@Mock`), taaki DB ya network na lage. `when / thenReturn` se nakli ko batata hoon kya lautana hai, `assertEquals` se
jaanchta hoon ki jo MILA wo sahi hai, `verify` se dekhta hoon ki jo HONA chahiye tha wo hua. Controller ke liye
`@WebMvcTest`, repository ki query ke liye `@DataJpaTest` (asli in-memory DB).

---

## Kahani: asli cook, nakli supplier

```
service    = COOK (asli)       -> @InjectMocks   asli UserService banti, uska code sach me chalta
repository = SUPPLIER (nakli)  -> @Mock          Mockito ek jhootha UserRepository banata, peeche DB nahi
@InjectMocks wala nakli supplier ko cook ke andar daal deta. Mock Mockito banata hai, Spring nahi.
```
Service DB tak sirf repository ke zariye pahunchti hai. Poora repository nakli kar diya to DB ka raasta hi band.

---

## Tera test, line-line

```java
@Mock        private UserRepository repository;   // nakli supplier
@InjectMocks private UserService service;         // asli cook

when(repository.findById(1L)).thenReturn(Optional.of(user));  // nakli ko RATWAYA: koi 1 maange to ye user de
User result = service.getById(1L);                            // asli service chali, andar nakli ko bulaya
assertEquals(user, result);                                   // jo MILA, wahi aaya?
verify(repository).findById(1L);                              // jo HONA tha (nakli ko bulana), wo hua?
```

**Confusion jo hota hai:** `when(repository.findById(1L))` padh ke lagta hai DB call ho gaya. Nahi hua: yahan `repository`
wahi nakli hai jo `@Mock` ne banaya. Ye line DB pe nahi jaati, bas nakli ko sikhati hai ki kya lautana hai. `@Mock`
hata ke asli repository lagao, tabhi DB jaata.

---

## Jab code sahi hai to test kyun?

Aaj code sahi hai to test hara hi aayega, ye sach hai. Test AAJ ke liye nahi, KAL ke liye hai: kal koi `getById` badle
(cache joda, `orElseThrow` hata ke `null` lauta diya, galti se do baar DB bula diya), to build pe turant laal ho jaata hai,
prod me pahunchne se pehle. Test = pehra, ek baar likho, har badlaav pe apne aap jaanch.

### assertEquals andar kya karta (chala ke dekha)
```
expected.equals(result) bulata hai. User pe Lombok @Data hai, to equals = saare field milao (id, name, email ...).

  same object (==)? false | equals? true      -> do alag object, field same -> PASS
  naam Ravi vs Arpan:
  expected: <User(id=1, name=Ravi, ...)> but was: <User(id=1, name=Arpan, ...)>   -> FAIL, farak saaf dikhta
```
Trap: class pe `@Data` / `equals` na ho to `equals` = same object hai ya nahi (`==`). Tab field same hone pe bhi FAIL.

### verify andar kya karta (chala ke dekha)
```
Nakli har bulaawe ko apni DIARY me likhta:  mock ki diary: [repository.findById(1L);]
verify = diary kholo, dekho ye call (isi argument ke saath) kitni baar hua.

  verify(repository, times(2)).findById(1L);   ->  Wanted 2 times:  But was 1 time:   -> FAIL
```
assert = kya MILA (output) · verify = kya HUA (call gaya ya nahi). `save` jaisa method jiska output kuch khaas nahi, wahan
asli jaanch verify hi hai ("save bulaya na?"). `times(2)`, `never()` bhi chalte.

---

## Asli fayda kahan

`getById` jaisa method jo bas laao aur lauta do karta, uska test kamzor: jo diya wahi aaya. Asli fayda wahan jahan service
kuch HISAAB kare: nakli ne price 100 diya, service 10% kaate, test 90 maange; kisi ne galti se 20% kaat diya (80) to laal.

---

## Tere project se (usercrud UserServiceTest, 4 PASS)
```
getById_whenUserExists_returnsUser  -> when(findById) user do; assertEquals + verify
getById_whenUserMissing_throws      -> when(findById) empty do; assertThrows(UserNotFoundException)   <- asli niyam jaancha
create_savesAndReturnsUser          -> when(save) user do; assertEquals; verify(save)
getAll_returnsList                  -> when(findAll) 2 user do; assertEquals(2, size)
```

---

## Trap: strict stubbing (chala ke dekha)
```java
when(repository.findById(1L)).thenReturn(...);
service.getById(2L);
```
```
PotentialStubbingProblem: Strict stubbing argument mismatch.
```
MockitoExtension sakht hai: nakli ko 1 ke liye sikhaya aur service ne 2 maanga to test seedha FAIL, chupchaap `empty` nahi.
Copy-paste me id badli, stub nahi badla = sabse aam galti.

---

## Testing ki seedhi (kaun kab)
```
UNIT        ek class akeli, dependency nakli (JUnit + Mockito). Tez, sabse zyada.
SLICE       ek layer asli: @WebMvcTest (controller + HTTP, service @MockBean, MockMvc se request)
                           @DataJpaTest (sirf repository, ASLI in-memory H2 DB, kyunki query khud jaanchni)
INTEGRATION @SpringBootTest: poora app chalu, sab asli. Dheema, sabse kam.
```
