package com.arpan.usercrud.dto;

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/* ════════════════════════════════════════════════════════════════════
 *  📝 RegisterRequest — New user signup JSON shape
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Yeh DTO POST /auth/register endpoint ka request body shape hai.
 *
 *  ─── 📨 EXPECTED JSON ─────────────────────────────────────────────
 *
 *      POST /auth/register
 *      Content-Type: application/json
 *
 *      {
 *          "name":     "Arpan",
 *          "email":    "arpan@x.com",
 *          "password": "secret123",
 *          "age":      28
 *      }
 *
 *      Note: "role" field NAHI — security ke liye!
 *      Default "USER" hum server side set karenge AuthController mein.
 *      Agar client se role accept karte → hacker "ADMIN" inject kar leta.
 *
 *  ─── 🛡️ VALIDATION ────────────────────────────────────────────────
 *
 *      • name     → 2-50 chars
 *      • email    → valid format + non-blank
 *      • password → minimum 6 chars
 *      • age      → 18-120 range
 *
 *  ─── 🎯 SECURITY: Why no "role" field? ────────────────────────────
 *
 *      ❌ BAD: Accept role from client
 *          Client → { "name": "X", ..., "role": "ADMIN" }
 *          Hacker self-promote kar leta admin
 *
 *      ✅ GOOD: Server hardcode role on register
 *          AuthController.register() mein:
 *              user.setRole("USER");   // always USER for new signups
 *          Admin manually upgrade kare future mein.
 * ════════════════════════════════════════════════════════════════════
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class RegisterRequest {

    @NotBlank(message = "Name is required")
    @Size(min = 2, max = 50, message = "Name must be 2-50 characters")
    private String name;

    @NotBlank(message = "Email is required")
    @Email(message = "Email must be valid")
    private String email;

    @NotBlank(message = "Password is required")
    @Size(min = 6, message = "Password must be at least 6 characters")
    private String password;

    @Min(value = 18, message = "Age must be at least 18")
    @Max(value = 120, message = "Age must be at most 120")
    private int age;
}
