package com.arpan.usercrud.dto;

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    LoginRequest = INPUT DTO for POST /auth/login
//    User credentials JSON shape
// ═══════════════════════════════════════════════════════════════════════
//
// EXPECTED JSON:
//    POST /auth/login
//    Content-Type: application/json
//    {
//        "email":    "arpan@x.com",
//        "password": "secret123"
//    }
//
// WHY DTO (not User entity)?
//    User has 6 fields (id, name, email, password, age, role)
//    Login needs ONLY 2 (email + password)
//    DTO = clean shape, no extra fields confusion
//
//    SECURITY:
//       Without DTO: hacker payload mein "role: ADMIN" inject kar le!
//       With DTO: only email + password accepted from client
//
// VALIDATION (triggers with @Valid in controller):
//    email    → @NotBlank + @Email (valid format)
//    password → @NotBlank (no length check — DB hash compare hota)
//
// FAIL FLOW:
//    Invalid input → MethodArgumentNotValidException → 400 Bad Request
//
// LOMBOK:
//    @Data              → getters/setters/toString/equals/hashCode
//    @AllArgsConstructor → all-fields constructor
//    @NoArgsConstructor → required for Jackson JSON deserialization
//
// SOLID — SRP: Sirf login input data
// ═══════════════════════════════════════════════════════════════════════

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
