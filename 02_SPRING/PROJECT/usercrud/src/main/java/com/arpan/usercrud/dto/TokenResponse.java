package com.arpan.usercrud.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    TokenResponse = OUTPUT DTO for login/register/refresh success
//    Successful authentication ke baad client ko deta
// ═══════════════════════════════════════════════════════════════════════
//
// EXPECTED JSON RESPONSE:
//    Status: 200 OK
//    Content-Type: application/json
//    {
//        "accessToken":  "eyJhbGc...short-lived...",
//        "refreshToken": "RRRR-...long-lived..."
//    }
//
// 🔑 WHY 2 TOKENS?
//    Access token (15 min)
//       • Har request mein bhejta
//       • Leak ho bhi gaya, sirf 15 min ka window
//
//    Refresh token (7 days)
//       • Sirf /refresh endpoint pe bhejta
//       • Kam exposure → safe to keep long-lived
//       • DB-stored = revoke karne ka option
//
//    Logout pe refresh DB se delete → "session" effectively end
//    Access token apne aap 15 min mein expire ho jata
//
// CLIENT BEHAVIOR (Frontend):
//    1. Both tokens save kar local storage / secure keystore:
//       localStorage.setItem("accessToken", response.accessToken);
//       localStorage.setItem("refreshToken", response.refreshToken);
//
//    2. Har subsequent API call mein accessToken bhejta:
//       headers: { Authorization: "Bearer " + accessToken }
//
//    3. Access token expire hua (401)?
//       Interceptor automatic /refresh hit karta
//       → naya accessToken paata
//
// RETURNED BY:
//    POST /auth/login    → both tokens
//    POST /auth/register → both tokens
//    POST /auth/refresh  → new pair (rotation)
//
// 📐 SOLID — SRP: Sirf token data carry karta
// ═══════════════════════════════════════════════════════════════════════

@Data
@AllArgsConstructor
@NoArgsConstructor
public class TokenResponse {

    // 🎫 Access token — short-lived (15 min)
    // Har protected API request mein send karna hai
    private String accessToken;

    // 🔄 Refresh token — long-lived (7 days)
    // Sirf /auth/refresh pe send karna hai jab access expire ho
    private String refreshToken;
}
