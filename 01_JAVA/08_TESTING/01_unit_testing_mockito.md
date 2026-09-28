# Unit Testing — JUnit + Mockito (mock/when/assert/verify)

> **Testing 101** — JP round me "how do you test?" aata. Real anchor = usercrud `UserServiceTest` (4 green).
> Ye JUnit5 + Mockito (unit test). Slice test (@WebMvcTest/@DataJpaTest) = neeche short.

---

## TESTING PYRAMID (kya-kab)
```
UNIT        -> ek class akeli; dependencies NAKLI (mock). tez, sabse zyada.   (JUnit + Mockito)
SLICE       -> ek layer real: @WebMvcTest (controller+HTTP), @DataJpaTest (repo+DB)
INTEGRATION -> @SpringBootTest (poora app up, sab real). slow, sabse kam.
```

---

## UNIT TEST ka DIL — jise test karein wo ASLI, uski dependency NAKLI
```
service    = ASLI (real UserService)   -> @InjectMocks   (real code chalta -> LOGIC test hota)
repository = NAKLI (mock)              -> @Mock          (DB/network se bacha)
```
- **@Mock** = Mockito ek fake `UserRepository` bana de
- **@InjectMocks** = wo fake, asli service ke andar daal de
- **@ExtendWith(MockitoExtension.class)** = Mockito chalu (NOTE: yahan SPRING hai hi nahi — mock **Mockito** banata, Spring nahi)

Analogy: service = **cook** (asli, sach me khana banata) · repository = **supplier** (nakli — tu khud "ye ingredient de" bolta). Cook ka khana sahi bana? = test.

---

## 3 KEYWORD (andar ka kaam)
```java
when(repository.findById(1L)).thenReturn(Optional.of(user));  // 1. SETUP: mock ko script do
User result = service.getById(1L);                            //    scene chalao (asli service)
assertEquals(user, result);                                   // 2. OUTPUT check: jo mila == expected?
verify(repository).findById(1L);                              // 3. INTERACTION check: method call hua?
```
- **when(x).thenReturn(y)** = mock ko sikhao "jab ye call aaye -> ye lauta". SETUP hai (verify nahi). Bina iske mock null lautaata.
- **assertEquals(exp, actual)** = OUTPUT sahi aaya? barabar->PASS, nahi->FAIL. ("kya MILA")
- **verify(mock).method()** = mock ka wo method sach me CALL hua? ("kya HUA / interaction"). count: `times(2)`, `never()`.

**assert vs verify (crux):** `assert` = OUTPUT (kya mila) · `verify` = INTERACTION (kya call hua). e.g. create me `verify(repo).save(user)` = "save call hua?", result nahi.

---

## ★ KOI DB CALL NAHI — line skip nahi, PURA object replace (deep)
Service KABHI seedha DB se baat nahi karta — sirf `repository` object se. Saara DB access repository ke through.
```
Mockito line-by-line scan/skip NAHI karta.
Bas: PURA repository = fake object banake service me daal deta.
-> service jitni baar bhi repository.xxx() bole -> har call FAKE ko jaati -> DB ka raasta hi nahi bacha.
```
Analogy: manager (service) khud warehouse (DB) nahi jaata — hamesha storekeeper (repository) ko phone. Storekeeper ki jagah ACTOR bitha diya -> manager 10 baar phone kare, har call actor ko, warehouse kabhi nahi. **Layered design (service->repository->DB) ki wajah se DB sirf repository se milta -> ek swap me saara DB access kat jaata.**
(2 repository ho -> dono @Mock; jo stub nahi kiya + call hua -> mock default null/empty lautaata.)

---

## ★★ TAUTOLOGY vs LOGIC-TEST (sabse gehri baat — interview me alag dikhata)
```
PASS-THROUGH method (getById: repo se laao, wahi laut do):
   input Arpan -> output Arpan (same object aar-paar) -> TAUTOLOGY -> kamzor test
   (sirf itna check: service ne repo call kiya + result return kiya, null/swallow nahi)

LOGIC-wala method (input pe KAAM hota):
   when(repo.findById).thenReturn(price=100)  // INPUT
   service.getPriceWithDiscount(1L)           // service 10% kaatta (LOGIC uske andar)
   assertEquals(90, result)                   // OUTPUT (input!=output) -> ASLI test
   -> bug ho (0.80 kaate->80) -> assert 90!=80 -> RED -> pakda
```
**Sabak:** trivial getter ka test tautology-jaisa (low value). Mock+assert ka asli faayda **logic-wale methods** pe — input do, transform-hone-ke-baad ka output assert karo. (10% ka math SERVICE ke andar hota, test sirf CHECK karta.)

---

## ★ PROJECT ANCHOR — usercrud UserServiceTest (4 green)
```
getById_whenUserExists_returnsUser  -> when(findById).thenReturn(user); assertEquals; verify
getById_whenUserMissing_throws      -> when(findById).thenReturn(empty); assertThrows(...)   <- ye MEANINGFUL (rule: empty->throw)
create_savesAndReturnsUser          -> when(save).thenReturn(user); assertEquals(user, service.create(user)); verify(save)
getAll_returnsList                  -> when(findAll).thenReturn(List.of(u1,u2)); assertEquals(2, size)
```
Test-data DUMMY (real email/password kabhi nahi). `service`=asli, `repository`=mock.

---

## SLICE TESTS (baad me — short)
```
@WebMvcTest(UserController.class)  -> sirf controller+HTTP layer; service @MockBean; MockMvc se request
                                      mockMvc.perform(get("/users/1")).andExpect(status().isOk())
@DataJpaTest                       -> sirf repository; REAL in-memory H2 DB (kyunki query khud test karni)
                                      -> ye wala DB use karta (unit test se ULTA)
```
```
unit test (service)  -> mock, DB NAHI
@DataJpaTest (repo)  -> real in-memory H2 (query test karne ko DB chahiye)
```

---

## POWER PHRASE
> *"For unit tests I use JUnit + Mockito — the class under test is real (@InjectMocks) and its dependencies are mocked (@Mock), so no DB or network. I stub behaviour with when/thenReturn, assert the output with assertEquals, and verify interactions with verify. Mocking the repository cuts off all DB access in one shot because the service only reaches the DB through it. Trivial getters barely need testing; the real value is on methods with logic where the input and expected output differ. For slices I use @WebMvcTest (controller + MockMvc) and @DataJpaTest (repository against in-memory H2)."*

> **Yaad rakh:**
> service = asli (@InjectMocks) · repository = mock (@Mock) · Mockito banata (Spring nahi)
> when=setup · assertEquals=output · verify=interaction
> DB nahi (object replace, line skip nahi) · pass-through=tautology, logic=asli test
> @WebMvcTest/@DataJpaTest = slice (DataJpaTest real H2 use karta)
