# GraphQL — Spring Boot me (hands-on, usercrud project)

> Ye maine KHUD usercrud project me lagaya, live GraphiQL me query maar ke dekha.
> Neeche: kya / kyun / kaise + poora code (4 step) + har cheez ka samajh.

---

## 0. GraphQL kya hai (1 line)
Ek **query-language for APIs**. **EK endpoint** (`POST /graphql`), aur **client khud batata kaunse field chahiye** → server utna hi bhejta.

## 1. KYUN (2 problem jo REST me hote, GraphQL solve karta)
1. **Over-fetching** — REST `GET /user/1` poora object (10 field) deta, chahe sirf `name` chahiye. Faltu data.
2. **Under-fetching / N+1 calls** — user + orders + items → REST me 3-4 alag call. GraphQL me **ek query** me sab nested.

## 2. 3 TUKDE (GraphQL ki jaan)
1. **SCHEMA** — server ka "menu" (types + fields + queries). CONTRACT.
2. **QUERY** — client bhejta ("menu se ye field chahiye"). Client fields chunta.
3. **RESOLVER** — server ka function ("ye field ka data kaha se laao").

---

## 3. SETUP — 4 STEP (jo humne kiya)

### STEP 1 — Dependency  [pom.xml]
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-graphql</artifactId>
</dependency>
```
- Version nahi (parent manage karta). Ye laata: graphql-engine + resolver-wiring + GraphiQL UI.
- (Jaise `spring-boot-starter-webmvc` REST ka setup deta, ye GraphQL ka.)

### STEP 2 — Schema  [src/main/resources/graphql/schema.graphqls]
> Folder EXACT `resources/graphql/` — Spring yahan se schema auto-load karta.
```graphql
type User {
    id: ID!
    name: String!
    email: String!
    age: Int!
    role: String!
}

type Query {
    users: [User!]!
    user(id: ID!): User
}
```
+ `application.properties` me test-UI on:
```
spring.graphql.graphiql.enabled=true
```

### STEP 3 — Resolver  [UserGraphQLController.java]
```java
@Controller     // NA @RestController — GraphQL alag hai
public class UserGraphQLController {

    private final UserRepository userRepository;
    public UserGraphQLController(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    @QueryMapping                       // method-naam = schema ka query-field (users)
    public List<User> users() {
        return userRepository.findAll();
    }

    @QueryMapping
    public User user(@Argument Long id) {   // @Argument = query ka argument (id) bind
        return userRepository.findById(id).orElse(null);
    }
}
```
- `@QueryMapping` = method ko schema ke **SAME-naam** query-field se jodta.
- `@Argument Long id` = GraphQL query ka `id` argument → method-param me aata.

### STEP 4 — Security permit + Run  [SecurityConfig.java]
GraphQL ke endpoints ko permit karo (JWT-security block na kare):
```java
.requestMatchers("/graphql", "/graphiql/**").permitAll()   // demo ke liye; production me auth-guard
```
Run: `mvn spring-boot:run` → app port **8080**.
Browser: **`http://localhost:8080/graphiql`** → query type karke ▶.

---

## 4. QUERIES (GraphiQL me chala ke dekha)
```graphql
{ users { name } }              # sirf naam
{ users { name email role } }   # naam + email + role
{ user(id: "1") { name age role } }   # ek user, chosen fields
```
**FARAK:** wahi ek endpoint, par **jo field maanga wahi aaya** — na kam, na zyada. (over-fetch khatam.)

**Response = JSON** (REST bhi JSON, GraphQL bhi). Magic REQUEST me hai (field-picking), response-format me nahi. GraphQL response shape:
```json
{ "data": { "users": [ { "name": "Test" } ] } }
```
(sab `data` key ke andar; galti ho to `errors` key me.)

---

## 4b. BEHIND-THE-SCENE — "koi alag API nahi dikhi?" (ek endpoint)
Confusion: REST me alag URL (`GET /users`, `GET /user/1`) → har URL "API" jaisa.
GraphQL me **EK endpoint** `POST /graphql` — sab queries usi pe (body me query). Isliye alag API nahi dikhti — **ek hi hai.**

**Jab GraphiQL me ▶ dabaaya, andar ye hua:**
```
1. GraphiQL ne query li: { users { name } }
2. HTTP POST banaya  -> POST localhost:8080/graphql
                        body: { "query": "{ users { name } }" }
3. Server (Spring GraphQL) -> SCHEMA se validate (field exist? type sahi?)
4. RESOLVER call -> users() -> userRepository.findAll() -> MySQL se data
5. Sirf maange field (name) nikaal -> JSON: { "data": { "users": [...] } }
6. GraphiQL ne JSON right-panel me dikhaya
```
Flow: **GraphiQL(client) -> POST /graphql (endpoint = "API") -> schema-validate -> resolver -> DB -> JSON wapas.**
- REST = bahut URL (har cheez alag). GraphQL = **ek URL**, query body me. Yehi GraphQL ka point.

---

## 5. SCHEMA kyun IMPORTANT (deep — key samajh)
Schema = client-server ka **CONTRACT**. 3 kaam:
1. **Contract + docs** — client ko pehle se pata kya maang sakta. **GraphiQL auto-complete isi schema se.**
2. **VALIDATION** — client ki query pehle schema se check hoti; galat field → error, resolver tak jaata hi nahi.
3. **Type-safety** — har field ka type fixed (String/Int/ID).

**Syntax:**
- `field: Type` — naam + type.
- `!` = **non-null** (bina `!` = null aa sakta). `[User!]!` = list; dono `!` = elements + list non-null.
- `type Query {}` = **reserved** — client sirf iske fields se query shuru karta.
- argument = `user(id: ID!)`.

## 6. ★ PASSWORD-SECURITY insight (khud test karke pakda)
`{ users { password } }` maanga → **turant red-underline ERROR** (bina run kiye).
- Kyun? `password` schema me likha hi NAHI (jaan-boojhke chhoda).
- **Do baat saabit:** (a) jo query karna hai wo **schema me likhna PADTA** — schema gatekeeper. (b) VALIDATION LIVE — server ko bheja bhi nahi, schema se hi error.
- **Security-faayda:** schema se **field-level control** — password schema me na daal ke expose hi nahi hone diya. Client kitna bhi maange, password bahar nahi ja sakta.

---

## 7. GraphQL vs REST (interview trade-off)
| | REST | GraphQL |
|---|---|---|
| Endpoints | bahut (`/users`, `/user/1`) | **ek** (`/graphql`) |
| Response | fixed (poora object) | **client picks fields** |
| Over/under-fetch | hota hai | khatam |
| Caching | HTTP-caching easy | mushkil (POST, dynamic) |
| Best for | simple/CRUD, cache-heavy | flexible clients (mobile, bandwidth-matter) |

## 8. INTERVIEW LINE
"GraphQL ek query-language hai — ek endpoint, client exact fields maangta (over/under-fetch khatam).
 3 tukde: schema (contract), query (client picks), resolver (data laata). Schema validation + type-safety deta,
 aur field-level expose-control (jaise password schema me na daal ke chhupaya). REST simpler + cache-friendly;
 GraphQL flexible clients ke liye. Maine Spring Boot me spring-boot-starter-graphql se schema + @QueryMapping
 resolver bana ke GraphiQL me test kiya."

## 9. DOBARA KAISE CHALAYE
```
usercrud/ me:  mvn spring-boot:run   (MySQL local chalu ho)
Browser:       http://localhost:8080/graphiql
Query:         { users { name email } }   ▶
```
(Files: pom dependency · resources/graphql/schema.graphqls · UserGraphQLController.java · SecurityConfig permit · application.properties graphiql=true)
