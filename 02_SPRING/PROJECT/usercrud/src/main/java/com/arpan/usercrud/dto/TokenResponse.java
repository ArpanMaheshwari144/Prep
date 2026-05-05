package com.arpan.usercrud.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/* ════════════════════════════════════════════════════════════════════
 *  🎫 TokenResponse — Login/Register/Refresh ka response shape
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Successful authentication ke baad client ko yeh dete hain.
 *  Login, Register, Refresh — sab same shape return karte.
 *
 *  ─── 📤 EXPECTED JSON RESPONSE ────────────────────────────────────
 *
 *      Status: 200 OK
 *      Content-Type: application/json
 *
 *      {
 *          "accessToken":  "eyJhbGc...short-lived...",
 *          "refreshToken": "eyJhbGc...long-lived..."
 *      }
 *
 *  ─── 🎯 CLIENT KYA KARTA YEH RESPONSE SE? ─────────────────────────
 *
 *      Frontend (browser/mobile):
 *
 *      1. Both tokens save kar local storage / secure keystore mein:
 *          localStorage.setItem("accessToken", response.accessToken);
 *          localStorage.setItem("refreshToken", response.refreshToken);
 *
 *      2. Har subsequent API call mein accessToken bhejta:
 *          headers: { Authorization: "Bearer " + accessToken }
 *
 *      3. Access token expire hua (15 min)?
 *         Interceptor automatic /refresh endpoint hit karta with
 *         refreshToken → naya accessToken paata.
 *
 *  ─── 🤔 KYU 2 TOKENS? ──────────────────────────────────────────────
 *
 *      • Access token (15 min) — har request mein bhejta. Leak ho
 *        bhi gaya, sirf 15 min ka window.
 *      • Refresh token (7 days) — sirf /refresh endpoint pe bhejta.
 *        Kam exposure → safe to keep long-lived.
 *
 *      Logout pe refresh token DB se delete → "session" effectively end.
 *      Access token apne aap 15 min mein expire ho jata.
 * ════════════════════════════════════════════════════════════════════
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class TokenResponse {

    /*
     *  🎫 Access token — short-lived (15 min)
     *  Har protected API request mein send karna hai.
     */
    private String accessToken;

    /*
     *  🔄 Refresh token — long-lived (7 days)
     *  Sirf /auth/refresh pe send karna hai jab access expire ho.
     */
    private String refreshToken;
}
