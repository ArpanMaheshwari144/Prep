# try-catch-finally

- **`try`:** jo kaam fail ho sakta hai.
- **`catch`:** fail hua to kya karna hai.
- **`finally`:** kuch bhi ho, ye chalega hi. Ye safai ke liye hai, jaise connection ya file band karna.

## Scene

```java
static int a() { try { return 1; } finally { print("finally chala"); } }
static int b() { try { return 1; } finally { return 2; } }
```

Chala ke dekha:

```
finally chala, return ke baad bhi
a() = 1
b() = 2
```

`return` ke baad bhi `finally` chala. Aur `finally` me khud `return` likh diya to try wala return dab gaya aur 2 aaya. Isliye
`finally` me `return` kabhi mat likho.

## Asli kaam ka tareeka: try-with-resources

DB connection ya file ko `finally` me haath se band karne ki jagah aise likhte hain:

```java
try (Conn c = getConnection()) { ... query ... }
```

```
connection band
pakda: query fail
```

Query fail hui, phir bhi connection apne aap band ho gaya. Band na ho to har fail request ek connection pakad ke rakh leti hai,
aur thodi der me HikariCP ka pool khaali ho jaata hai, phir naye request ko connection milta hi nahi.

## Trap

`catch (Exception e)` pehle aur `catch (IOException e)` uske baad likha to compile error aata hai. Chhota (specific) catch
hamesha upar aata hai.
