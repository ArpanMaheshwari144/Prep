package com.arpan.usercrud.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    RefreshRequest = INPUT DTO for /auth/refresh + /auth/logout
//    Used by 2 endpoints (both need refresh token in body)
// ═══════════════════════════════════════════════════════════════════════
//
// EXPECTED JSON:
//    POST /auth/refresh   (ya /auth/logout)
//    Content-Type: application/json
//    {
//        "refreshToken": "RRRR-..."
//    }
//
// USE CASE 1 — REFRESH FLOW:
//    Client: access token expire ho gaya
//         ↓
//    POST /auth/refresh { refreshToken: "RRRR..." }
//         ↓
//    Server: DB mein refreshToken validate
//         ↓
//    Naya access + refresh return → TokenResponse
//
// USE CASE 2 — LOGOUT FLOW:
//    Client: logout button click
//         ↓
//    POST /auth/logout { refreshToken: "RRRR..." }
//         ↓
//    Server: refreshTokenRepo.deleteByToken("RRRR")
//         ↓
//    Client: localStorage.clear()
//         ↓
//    Result: refresh token DB nahi → next refresh = 401
//            access token 15 min mein expire (auto)
//
// VALIDATION:
//    Sirf @NotBlank — token presence check
//    Actual format/signature server side validate hota (JwtService)
//
// 📐 SOLID — SRP: Sirf refresh token input
// ═══════════════════════════════════════════════════════════════════════

@Data
@AllArgsConstructor
@NoArgsConstructor
public class RefreshRequest {

    @NotBlank(message = "Refresh token is required")
    private String refreshToken;
}
