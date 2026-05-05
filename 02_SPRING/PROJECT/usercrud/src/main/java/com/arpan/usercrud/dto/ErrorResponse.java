package com.arpan.usercrud.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

/* ════════════════════════════════════════════════════════════════
 *  📌 DTO — Data Transfer Object (Error Response shape)
 * ════════════════════════════════════════════════════════════════
 *  Yeh class ek consistent error response shape define karti hai
 *  jo client ko milega jab kuch galat ho.
 *
 *  Saare error responses iss format mein:
 *  {
 *      "status": 404,
 *      "message": "User not found with id: 5",
 *      "timestamp": 1706443200000
 *  }
 *
 *  ─── DTO vs ENTITY (interview puchega) ──────────────────────────
 *  Entity (User)         → DB se mapped, ORM-ready
 *  DTO    (ErrorResponse)→ API ke beech data ka shape (DB se nahi)
 *
 *  Production mein DTO patterns use kyu?:
 *  • Entity ke andar DB-only fields (password hash, internal flags)
 *    bahar nahi jaane chahiye → DTO pe sirf required fields
 *  • API contract decouple — entity badle toh API na toote
 *  • Validation rules alag — incoming DTO pe @NotBlank, entity pe @Column
 *
 *  ─── Lombok shortcuts ───────────────────────────────────────────
 *  @Data              → getters + setters + toString + equals + hashCode
 *  @AllArgsConstructor→ saare fields wala constructor
 * ════════════════════════════════════════════════════════════════
 */
@Data
@AllArgsConstructor
public class ErrorResponse {
    private int status;       // HTTP status code (400, 404, 500)
    private String message;   // Human-readable error message
    private long timestamp;   // Milliseconds since epoch (debug ke liye)
}
