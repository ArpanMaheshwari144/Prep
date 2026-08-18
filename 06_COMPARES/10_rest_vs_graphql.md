# REST vs GraphQL

---

## 1-Line Analogy

```
REST    = Fixed menu     (order item A, item B separately)
GraphQL = Custom chef    ("give me exactly what I ask")
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  REST             │  GraphQL          │
├──────────────────┼──────────────────┼──────────────────┤
│ Endpoints        │ Many (per resource│ ONE single        │
│                  │ /users, /posts)    │ /graphql           │
├──────────────────┼──────────────────┼──────────────────┤
│ HTTP methods     │ GET/POST/PUT/DEL  │ Mostly POST       │
├──────────────────┼──────────────────┼──────────────────┤
│ Over-fetching    │ Yes             │ No (client      │
│                  │ (server decides)   │ picks fields)     │
├──────────────────┼──────────────────┼──────────────────┤
│ Under-fetching   │ Yes             │ No (nested in   │
│                  │ (need multiple req)│ one query)        │
├──────────────────┼──────────────────┼──────────────────┤
│ Schema           │ OpenAPI/Swagger   │ Built-in strong   │
│                  │ optional           │ typed schema      │
├──────────────────┼──────────────────┼──────────────────┤
│ Caching          │ Easy (URL-based)│ Hard           │
│                  │ Browser caches      │ (custom logic)    │
├──────────────────┼──────────────────┼──────────────────┤
│ Versioning       │ /v1/users → /v2/ │ Schema evolves    │
│                  │                     │ (deprecate fields)│
├──────────────────┼──────────────────┼──────────────────┤
│ Learning curve   │ Simple          │ Steeper        │
├──────────────────┼──────────────────┼──────────────────┤
│ N+1 problem      │ Manageable         │ Common (need      │
│                  │                     │ DataLoader)       │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## REST EXAMPLE

```
TWITTER-LIKE APP:

1. Get user info:
   GET /users/123
   Response: { id, name, email, age, address, phone, ... 30 fields }

2. Get user's tweets:
   GET /users/123/tweets
   Response: [{ tweet1 }, { tweet2 }, ...]

3. Get each tweet's likes:
   GET /tweets/1/likes
   GET /tweets/2/likes
   GET /tweets/3/likes
   ...

= 3+ separate requests
= OVER-FETCHING (got 30 fields, needed 3)
= UNDER-FETCHING (need 3 requests for full data)
```

---

## GRAPHQL EXAMPLE (Same data)

```
POST /graphql
{
  user(id: 123) {
    name              ← only fields needed
    tweets {
      content
      likes {
        count
      }
    }
  }
}

Response:
{
  "data": {
    "user": {
      "name": "Arpan",
      "tweets": [
        { "content": "Hi", "likes": { "count": 5 } },
        { "content": "Bye", "likes": { "count": 3 } }
      ]
    }
  }
}

= 1 request
= Exact data needed
= No over/under fetching
```

---

## Over-Fetching Visual

```
REST:
   Client needs: name + email

   GET /users/123
   Server returns: {
      name: "Arpan",        ← needed
      email: "a@b.com",     ← needed
      phone: "...",         ← extra
      address: "...",       ← extra
      bio: "...",            ← extra
      preferences: {...},    ← extra
      ... 30 more fields    ← extra
   }

   = Server decides, client gets MORE than needed

GraphQL:
   Client asks: { user(id:123) { name email } }
   Server returns: { name, email }

   = Client decides, gets EXACTLY what asked
```

---

## Under-Fetching Visual

```
REST:
   Need: User profile + their 5 latest tweets

   Request 1: GET /users/123 → user data
   Request 2: GET /users/123/tweets → tweets

   = 2 round trips
   = Mobile network = slow

GraphQL:
   query {
      user(id: 123) {
         name
         tweets(limit: 5) { content }
      }
   }

   = 1 round trip
   = Mobile friendly
```

---

## GRAPHQL STRONG TYPING

```
GraphQL SCHEMA (mandatory):

   type User {
     id: ID!
     name: String!
     email: String
     tweets: [Tweet!]!
   }

   type Tweet {
     id: ID!
     content: String!
     likes: Int!
     author: User!
   }

   type Query {
     user(id: ID!): User
     tweets: [Tweet!]!
   }

= Self-documenting
= Auto-completion in IDE
= Compile-time errors
= Client/server contract enforced
```

---

## GRAPHQL CHALLENGES

```
1. N+1 PROBLEM (common pitfall)
   Query: { users { name posts { title } } }

   Naive resolver:
      1 query for all users
      Then 1 query per user for posts
      = N+1 queries (slow)

   FIX: DataLoader (batch + cache)

2. CACHING HARD
   REST: GET /users/123 → cache by URL
   GraphQL: POST /graphql → same URL, different queries
   = Cache key must include query
   = Browser caching broken

3. RATE LIMITING HARD
   REST: count requests
   GraphQL: complex query = "1 request" but expensive
   = Need query complexity analysis
   = Depth limits, field limits

4. ERROR HANDLING
   REST: HTTP status codes (404, 400, 500)
   GraphQL: Always 200 OK, errors in body
   = Different mental model

5. FILE UPLOAD
   REST: multipart/form-data → standard
   GraphQL: needs extension/workaround
```

---

## WHEN TO USE WHAT

```
REST CHOOSE WHEN:
   • Simple CRUD APIs
   • Public APIs (broad reach)
   • Strong HTTP caching needed
   • CDN-friendly
   • Microservices (each owns its API)
   • Team unfamiliar with GraphQL

   Examples:
   • Stripe API
   • Twitter v1 (now v2)
   • Most enterprise APIs
   • UserCRUD

GRAPHQL CHOOSE WHEN:
   • Mobile apps (over-fetching costly)
   • Complex nested data
   • Multiple client types (web, mobile, IoT)
   • Frontend team wants flexibility
   • Aggregating multiple data sources
   • Rapid frontend iteration

   Examples:
   • Facebook (invented it)
   • GitHub API v4
   • Shopify
   • Twitter v2 (GraphQL)
```

---

## REAL-WORLD STORY

```
Facebook 2012:
   Mobile app slow on 2G networks
   REST API = too many requests, over-fetching

   Solution: Created GraphQL internally

   Result:
   • Single request per screen
   • Mobile-friendly bandwidth
   • Frontend devs autonomous (pick fields)

   2015: Open-sourced
   Now: GitHub, Shopify, Airbnb, Twitter, Pinterest
```

---

## HYBRID — Best of Both

```
Modern stack often uses BOTH:

   REST for:
   • Auth (/login, /logout)
   • File uploads
   • Webhooks
   • Public APIs
   • Cacheable resources

   GraphQL for:
   • Internal frontend data fetching
   • Mobile app queries
   • Complex aggregation

= Not "either or"
= Tool for the job
```

---

## INTERVIEW LINE

```
"REST has multiple endpoints, one per resource, with
 standard HTTP methods. Simple, cacheable, but causes
 over-fetching and under-fetching.

 GraphQL has a single endpoint, client specifies exactly
 what fields needed, no over/under fetching, strong schema.
 But caching is harder, N+1 problem common, learning curve.

 REST for simple CRUD APIs and public APIs.
 GraphQL for mobile apps with complex nested data and
 multiple client types.

 Facebook invented GraphQL to solve mobile bandwidth
 problems. Modern stacks often use both — REST for
 cacheable resources, GraphQL for internal frontend."
```

---

## Memory Hook

```
REST    = Fixed menu (multiple endpoints, server decides)
GraphQL = Custom chef (one endpoint, client decides)

REST: simple, cacheable, over/under fetching
GraphQL: efficient, flexible, N+1 risk + cache hard

Mobile-heavy → GraphQL
Public/simple → REST

Facebook invented GraphQL (mobile pain)
DataLoader = N+1 fix
```

[← HLD README](../README.md)
