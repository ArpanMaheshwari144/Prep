package com.arpan.usercrud.dto;

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/* ════════════════════════════════════════════════════════════════════
 *  📝 LoginRequest — User credentials JSON shape
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Yeh DTO POST /auth/login endpoint ka request body shape hai.
 *
 *  ─── 📨 EXPECTED JSON ─────────────────────────────────────────────
 *
 *      POST /auth/login
 *      Content-Type: application/json
 *
 *      {
 *          "email":    "arpan@x.com",
 *          "password": "secret123"
 *      }
 *
 *  ─── 🛡️ VALIDATION (with @Valid in controller) ────────────────────
 *
 *      • email    → @NotBlank + @Email (valid format)
 *      • password → @NotBlank (no length check yahaan — DB mein hash hi compare hota)
 *
 *      Fail hua → MethodArgumentNotValidException → 400 Bad Request
 *
 *  ─── 🎯 WHY DTO not Entity? ───────────────────────────────────────
 *
 *      Entity (User) mein 6 fields hain — id, name, age, role, etc.
 *      Login ke liye sirf email + password chahiye.
 *      DTO clean shape — bina extra fields confusion ke.
 *
 *      Plus security: agar User entity directly accept karte, hacker
 *      "role: ADMIN" inject kar sakta payload mein — DTO yeh attack
 *      surface nahi rakhta.
 * ════════════════════════════════════════════════════════════════════
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class LoginRequest {

    @NotBlank(message = "Email is required")
    @Email(message = "Email must be valid")
    private String email;

    @NotBlank(message = "Password is required")
    private String password;
}
