# Wrapper classes (Integer, Long)

## Scene: owner check, test me pass, prod me 403

Code me likha hai `if (order.getUserId() == currentUser.getId())`, aur dono `Long` hain. Test me user id 100 thi, production
me 5000. Chala ke dekha:

```
id 100:  true
id 5000: false   equals: true
```

Test pass ho gaya, par production me asli maalik ko bhi 403 mil gaya. Java -128 se 127 tak ke chhote number ke object pehle se
bana ke rakhta hai aur wahi baar-baar deta hai, isliye 100 pe dono same object nikle. 5000 pe har baar naya object banta hai,
aur `==` sirf object milata hai, number nahi. Ye bilkul String wala `==` trap hai, aur ilaaj bhi wahi hai: `equals()`.

## Khaas baat

`List<int>` nahi chalta, kyunki collection me sirf object jaate hain, isliye `Integer` chahiye. `int` se `Integer` aur
`Integer` se `int` ka badlaav Java khud kar deta hai, ise autoboxing aur unboxing kehte hain.

## Trap 2

`int c = map.get("x");` likha aur key thi hi nahi. Map ne `null` diya, Java ne use `int` banane ki koshish ki, to
`NullPointerException` aaya. Ilaaj: `map.getOrDefault("x", 0)`.

## Interview line

```
Wrapper classes like Integer and Long let primitives live in collections, and Java boxes
and unboxes automatically. Integer caches -128 to 127, so == works for small ids and fails
for big ones; always use equals. Unboxing a null throws NullPointerException.
```
