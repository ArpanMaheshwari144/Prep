# StringBuilder vs StringBuffer

Ye pichhli file (String immutable) ka hi agla hissa hai.

## Scene: report export, 1 lakh ids ek CSV line me

DB ki 1 lakh rows ki ids ek line me jodni hain. Ek bande ne `String` se joda, doosre ne `StringBuilder` se:

```java
String csv = "";
for (int i = 0; i < 100000; i++) csv += i + ",";

StringBuilder sb = new StringBuilder();
for (int i = 0; i < 100000; i++) sb.append(i).append(",");
```

Chala ke dekha:

```
String +=      : 3909 ms, length 588890
StringBuilder  : 1 ms, length 588890
```

Dono ka nateeja same hai, par ek ko 4 second lage aur doosre ko 1 ms. Wajah pichhli file me hi hai: String badalti nahi. Har
`+=` pe Java poori purani line copy karke ek nayi String banata hai, matlab 1 lakh baar copy, aur har baar line pehle se
lambi. StringBuilder ke andar ek badhne wala dabba (array) hota hai jisme tu seedha aage jodta jaata hai. Copy sirf tab hoti
hai jab dabba bhar jaaye.

```
String +=      : [1,] -> [1,2,] -> [1,2,3,] ...   har baar naya object, poora copy
StringBuilder  : [1,2,3,4,....______]              ek hi dabba, khaali jagah me aage likho
```

## StringBuffer kyun hai

StringBuffer bilkul StringBuilder jaisa hai, bas uska har method `synchronized` hai, matlab ek waqt me ek hi thread andar ja
sakta hai. Do thread se ek hi dabba bharwaya, dono ne 1-1 lakh "x" joda:

```
StringBuilder: 192667   StringBuffer: 200000   (chahiye 200000)
```

StringBuilder me lagbhag 7 hazaar akshar kho gaye, kyunki dono thread ek saath likh rahe the. StringBuffer me sab pahunche,
par har append pe lock ka kharcha laga.

## Khaas baat

Ek line ka `a + b + c` compiler khud theek kar deta hai, isliye dikkat sirf LOOP me aati hai.

Asal kaam me 99% StringBuilder hi chalta hai, kyunki wo method ke andar ka local variable hota hai aur har thread ka apna hota
hai. Isliye StringBuffer ka kaam bahut kam padta hai.

## Prod wala trap

Spring ki service singleton hoti hai. Kisi ne class ke field me `private StringBuilder sb = new StringBuilder();` rakh diya
taaki log ya message banaye. Ab do request ek saath aayi to dono ek hi dabba bharengi, aur ek user ke message me doosre ka
text chala jaayega. Ye wahi "singleton me field" wali galti hai jo PR drill me pakadte hain. Ilaaj: StringBuilder method ke
andar banao.

## Interview line

```
String concatenation inside a loop creates a new object every time, so it is O(n^2) copying;
StringBuilder appends into one growing buffer, which is why a 1 lakh row CSV went from
4 seconds to 1 ms. StringBuffer is the same but synchronized, so it is thread safe and slower.
In practice I keep StringBuilder as a local variable, so each thread has its own and
StringBuffer is rarely needed; a StringBuilder field in a singleton bean is a bug.
```
