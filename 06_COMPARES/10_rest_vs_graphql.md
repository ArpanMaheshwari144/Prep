# REST vs GraphQL

GraphQL kya hai aur usercrud me kaise lagaya = [Spring 11 GraphQL](../02_SPRING/11_GRAPHQL/01_graphql.md) ("jitna maango utna deta").
Yahan sirf kab kya, aur GraphQL ki keemat.

## Scene: Facebook 2012

Facebook ka mobile app 2G pe ghisat raha tha. Ek screen ke liye 4-5 REST call lagti thi, aur har call me 30 field aate the. Isliye
Facebook ne GraphQL banaya, aur 2015 me sabke liye khol diya. Aaj GitHub ki API v4 aur Shopify GraphQL pe hain; Stripe aur zyadatar
enterprise API REST pe.

## Kab kya

- **REST:** seedha CRUD, public API, aur jahan cache chahiye. Har cheez ka alag URL (`/users`, `/users/123/tweets`), GET / POST / PUT /
  DELETE. Tera usercrud REST hai.
- **GraphQL:** mobile app, ek screen pe bahut judi hui cheezein (user + tweets + likes ek query me), aur kai tarah ke client (web,
  mobile) jinhe alag-alag field chahiye. Ek hi URL `/graphql`, client field chunta hai.
- Asal me dono saath chalte hain: login, file upload aur webhook REST pe, app ke andar ka data GraphQL se.

## GraphQL ki keemat (HLD se jod)

1. **Cache:** REST me `GET /users/123` ka URL hi cache ki chaabi hai, jaise URL shortener aur Netflix me CDN URL dekh ke rakhta. GraphQL
   me sab ek hi URL pe `POST`, to URL dekh ke cache nahi kar sakte.
2. **Rate limiter:** rate limiter me "ek request = ek token" tha. GraphQL me ek hi request `{ users { orders { items } } }` 1000 REST call
   jitni bhaari ho sakti hai. Isliye wahan request ginne ki jagah query kitni gehri / bhaari hai wo naapte hain (depth / complexity limit).
3. **Galti:** REST me 404 / 500 se pata chalta hai. GraphQL me galti aksar 200 ke andar `errors` me aati hai (Spring GraphQL bhi aise hi
   bhejta hai), to monitoring alag se dekhni padti hai.

## Prod wala trap: N+1

Query aayi `{ users { orders { id } } }`. Resolver ne 1 query me 100 users laaye, phir har user ke orders ke liye alag query chalayi.
Kul 1 + 100 = 101 DB query, ek hi request me, aur client ko pata bhi nahi (usne to ek query bheji). Tere usercrud ka `/n1` demo isi dikkat
ka hai.

**Hal:** sabke orders ek saath maango (`WHERE user_id IN (...)`). Spring GraphQL me `@BatchMapping`, bahar ki duniya me iska naam DataLoader.

[← COMPARES README](../README.md)
