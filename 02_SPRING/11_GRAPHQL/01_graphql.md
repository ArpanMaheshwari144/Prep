# GraphQL (usercrud me khud lagaya, GraphiQL me chala ke dekha)

**Arpan ka nichod:** GraphQL ek aisi cheez hai jo jitna maango utna deta hai, usse zyada nahi. Bas yahi hai.

## Scene: Ravi gaon me 3G pe

Ravi gaon me 3G pe hai (Twitter ke kadam 11 wala, Alto wala net). App ki profile screen ko sirf naam aur photo chahiye, par REST ka
`GET /user/1` poora object bhej deta hai, 10 field, aur dheeme net pe sab kheenchna padta hai. Isko over-fetching kehte hain. Ulta bhi
hota hai: home screen ko user, uske orders aur har order ke items chahiye, to REST me 3-4 alag call lagti hain (under-fetching).

GraphQL me ek hi endpoint hai, `POST /graphql`. Client khud batata hai kaunse field chahiye, aur server utna hi bhejta hai.

## Chala ke dekha (usercrud, GraphiQL)

```graphql
{ users { name } }                      ->  { "data": { "users": [ { "name": "Test" } ] } }
{ users { name email role } }           ->  sirf ye teen field
{ user(id: "1") { name age role } }     ->  ek user, chune hue field
```

Response JSON hi hai, REST jaisa. Farak request me hai: field client chunta hai. Jawab hamesha `data` key ke andar aata hai, aur galti
ho to `errors` key me.

Teen tukde hain. **Schema** (`schema.graphqls`) server ka menu hai, jo maang sakte ho. **Query** me client menu se chunta hai.
**Resolver** (`@QueryMapping` wala method) data laata hai, aur uska naam schema ke field jaisa hi hota hai: `users()` -> `users`.

## Chhoti dikkat jo khud pakdi: password

`{ users { password } }` maanga to bina chalaye hi laal line aa gayi, kyunki `password` schema me likha hi nahi tha. Schema gatekeeper
hai: jo field schema me nahi, wo client kitna bhi maange, bahar nahi jaata. Query pehle schema se jaanchi jaati hai, galat field resolver
tak pahunchta hi nahi. Field-level control isi se milta hai.

## Setup jo usercrud me kiya (4 kadam)

1. `pom.xml` me `spring-boot-starter-graphql` (version parent sambhalta hai). Ye engine, resolver ki wiring aur GraphiQL UI laata hai.
2. Schema `src/main/resources/graphql/schema.graphqls` me. Folder ka naam ekdum yahi hona chahiye, Spring yahin se uthata hai.
   `application.properties` me `spring.graphql.graphiql.enabled=true`.

```graphql
type User { id: ID!  name: String!  email: String!  age: Int!  role: String! }
type Query { users: [User!]!  user(id: ID!): User }
```

3. Resolver [UserGraphQLController.java](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/UserGraphQLController.java):

```java
@Controller                              // @RestController nahi
public class UserGraphQLController {
    private final UserRepository userRepository;
    public UserGraphQLController(UserRepository userRepository) { this.userRepository = userRepository; }

    @QueryMapping                        // naam = schema ka field "users"
    public List<User> users() { return userRepository.findAll(); }

    @QueryMapping
    public User user(@Argument Long id) { return userRepository.findById(id).orElse(null); }   // @Argument = query ka id
}
```

4. `SecurityConfig` me `/graphql` aur `/graphiql/**` permit kiye, phir `mvn spring-boot:run` (MySQL chalu ho) aur browser me
   `localhost:8080/graphiql`, query likh ke chalao.

`!` ka matlab null nahi aa sakta. `[User!]!` me list bhi null nahi aur andar ka koi element bhi null nahi.

## Prod wala trap (JWT se jod)

```java
.requestMatchers("/graphql", "/graphiql/**").permitAll()   // demo ke liye
```

Ye line prod me chali gayi to bina token koi bhi `{ users { name email role } }` maar ke saare users ki list le jaayega, kyunki ek hi
endpoint pe saari queries hain. REST me har URL alag lock hota hai, yahan ek darwaza khula matlab sab khula. Prod me `/graphql`
authenticated rakho (JWT wala filter) aur GraphiQL band. Aur HTTP caching REST jitni aasan nahi, kyunki sab POST pe ek URL se jaata hai.
