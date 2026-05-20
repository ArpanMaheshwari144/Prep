package com.arpan.todoapp.dto;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/dto/TokenResponse.java

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    TokenResponse = LOGIN ka HTTP response shape
//    POST /auth/login → 200 OK body:
//       {
//          "accessToken": "eyJhbGc...",
//          "refreshToken": "eyJhbGc..."
//       }
//
//    Client stores both tokens
//    Uses accessToken in Authorization header for API calls
//    Uses refreshToken to get new accessToken when expired
// ═══════════════════════════════════════════════════════════════════════
//
// ACCESS TOKEN vs REFRESH TOKEN:
//
//    ACCESS TOKEN (short-lived):
//       Validity: 15 min (jwt.expiration in properties)
//       Use: Every API call (Authorization: Bearer xxx)
//       If stolen: damage limited to 15 min
//
//    REFRESH TOKEN (long-lived):
//       Validity: 7 days typically (longer)
//       Use: Get new access token when current expires
//       Endpoint: POST /auth/refresh
//       If stolen: bigger risk, hence stored DB-side (revocable)
//
//    WHY 2 TOKENS:
//       Short access token = security (small attack window)
//       Long refresh token = UX (no re-login every 15 min)
//       Best of both
//
// NO VALIDATION (response DTO):
//    @NotBlank etc. are for INCOMING requests
//    Outgoing response always populated by server
//    No validation needed
//
// RESPONSE-ONLY DTO:
//    No constructor logic, just data shape
//    Jackson serializes to JSON automatically

@Data
@NoArgsConstructor
@AllArgsConstructor
public class TokenResponse {

    // ─── ACCESS TOKEN (short-lived, 15 min) ─────────────────
    // Used in every API request:
    //    Authorization: Bearer <accessToken>
    private String accessToken;

    // ─── REFRESH TOKEN (long-lived, 7 days) ─────────────────
    // Used to get NEW access token when current expires
    //    POST /auth/refresh { refreshToken }
    // Note: actual refresh-token rotation logic in AuthController
    private String refreshToken;
}
