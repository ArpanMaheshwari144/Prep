package com.arpan.usercrud.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/* ════════════════════════════════════════════════════════════════════
 *  🔄 RefreshRequest — Token refresh ka request body shape
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Yeh DTO POST /auth/refresh aur /auth/logout endpoints ka
 *  request body shape hai.
 *
 *  ─── 📨 EXPECTED JSON ─────────────────────────────────────────────
 *
 *      POST /auth/refresh   (ya /auth/logout)
 *      Content-Type: application/json
 *
 *      {
 *          "refreshToken": "eyJhbGc...long-lived..."
 *      }
 *
 *  ─── 🎬 USE CASES ─────────────────────────────────────────────────
 *
 *      1. REFRESH FLOW:
 *         Client access token expire ho gaya
 *              ↓
 *         POST /auth/refresh { refreshToken: "RRRR..." }
 *              ↓
 *         Server DB mein refreshToken validate
 *              ↓
 *         Naya access token return → TokenResponse
 *
 *      2. LOGOUT FLOW:
 *         Client logout button click
 *              ↓
 *         POST /auth/logout { refreshToken: "RRRR..." }
 *              ↓
 *         Server: refreshTokenRepo.deleteByToken("RRRR")
 *              ↓
 *         Client: localStorage.clear()
 *              ↓
 *         Result: refresh token DB mein nahi → next refresh = 401
 *                 access token 15 min mein expire
 *
 *  ─── 🛡️ VALIDATION ────────────────────────────────────────────────
 *
 *      Sirf @NotBlank — token presence check.
 *      Actual JWT format/signature server side validate hota.
 * ════════════════════════════════════════════════════════════════════
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class RefreshRequest {

    @NotBlank(message = "Refresh token is required")
    private String refreshToken;
}
