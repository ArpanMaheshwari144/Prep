package com.arpan.usercrud.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    ErrorResponse = OUTPUT DTO for ALL error responses
//    Consistent error shape client ko milega when something fails
// ═══════════════════════════════════════════════════════════════════════
//
// EXPECTED JSON (ALL errors):
//    {
//        "status": 404,
//        "message": "User not found with id: 5",
//        "timestamp": 1706443200000
//    }
//
// USED BY:
//    GlobalExceptionHandler (@RestControllerAdvice)
//    Sab exceptions iss shape mein convert hote:
//       UserNotFoundException → 404 + message + timestamp
//       Validation errors     → 400 + message + timestamp
//       Generic               → 500 + message + timestamp
//
// WHY CONSISTENT SHAPE?
//    Frontend ko predictable structure milti
//    Error handling logic ek hi tarah likh sakte:
//       if (response.status === error) {
//           showAlert(response.message);
//       }
//
// SOLID — SRP: Sirf error data carrier
// ═══════════════════════════════════════════════════════════════════════

@Data
@AllArgsConstructor
public class ErrorResponse {
    private int status;       // HTTP status code (400, 404, 500)
    private String message;   // Human-readable error message
    private long timestamp;   // Milliseconds since epoch (debug ke liye)
}
