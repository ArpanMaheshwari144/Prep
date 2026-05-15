package com.arpan.usercrud.exception;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

import com.arpan.usercrud.dto.ErrorResponse;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    GLOBAL EXCEPTION HANDLER — Centralized Error Routing
//    Saare controllers ki exceptions yahaan flow hoti
//    Consistent JSON error response milta
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — WITHOUT vs WITH:
//    WITHOUT Handler:
//       Har controller method mein:
//          try { service.findById(id); }
//          catch (UserNotFoundException e) { return 404; }
//          catch (Exception e) { return 500; }
//       = Repetitive everywhere
//
//    WITH @RestControllerAdvice:
//       Controller saaf:
//          return service.findById(id);
//       Exception thrown → centrally caught here → JSON returned
//
// 🔑 @RestControllerAdvice DECODED:
//    = @ControllerAdvice + @ResponseBody
//
//    @ControllerAdvice → "Saare controllers ke liye shared advice
//                         (cross-cutting concerns)"
//    @ResponseBody     → "Return value DIRECT JSON banao"
//
//    Without @ResponseBody:
//       Spring soche ErrorResponse view name hai → 500 error
//
// 🔑 @ExceptionHandler(SomeException.class):
//    "Jab koi controller SomeException throw kare →
//     yeh method call karo response banane ke liye"
//
//    Spring sabse SPECIFIC handler dhundhta hai:
//       UserNotFoundException → use UserNotFound handler
//       (Exception ka generic handler skip)
//       = Polymorphism + reflection magic
//
// HANDLER PRIORITY (specific → generic):
//    UserNotFoundException  →  404 (most specific)
//           │
//    IllegalArgumentException → 400
//           │
//    Exception              →  500 (catch-all fallback)
//
// HTTP STATUS CHEAT SHEET:
//    200 OK            → success with body
//    201 Created       → POST success (resource banaya)
//    204 No Content    → DELETE success
//    400 Bad Request   → client galti (invalid data)
//    401 Unauthorized  → login chahiye
//    403 Forbidden     → login hai par permission nahi
//    404 Not Found     → resource missing
//    500 Server Error  → server side bug / unhandled
//
// RESPONSE FLOW VISUAL:
//    Service throws UserNotFoundException
//         │
//         ▼ bubbles up
//    Controller (doesn't catch)
//         │
//         ▼
//    Spring DispatcherServlet catches
//         │
//         ▼ matches @ExceptionHandler
//    GlobalExceptionHandler.handleUserNotFoundException()
//         │
//         ▼
//    ResponseEntity<ErrorResponse> with 404
//         │
//         ▼
//    Client:
//    {
//      "status": 404,
//      "message": "User not found with id: 5",
//      "timestamp": 1706443200000
//    }
//
// ⚠️ PRODUCTION TIP (Interview Gold):
//    Generic Exception handler:
//       ❌ DON'T leak ex.getMessage() to client
//          (stack trace, internal info leak = security risk)
//       ✅ Log full exception:
//             logger.error("Unhandled", ex);
//       ✅ Return GENERIC user message:
//             "An unexpected error occurred. Please try again."
//       = Security best practice
//
// 📐 SOLID:
//    SRP — Sirf exception → HTTP response mapping
//          No business logic here
//
//    OCP — Naya exception type aaye?
//          Just add @ExceptionHandler method
//          Existing handlers UNCHANGED
//          = Open for extension
//
// 🎤 INTERVIEW LINE:
//    "@RestControllerAdvice centralizes exception handling — Spring
//     catches exceptions from any controller and routes to matching
//     @ExceptionHandler method.
//
//     Handler priority: most specific wins (UserNotFound → 404
//     before generic Exception → 500).
//
//     Production caveat: don't leak ex.getMessage() in generic
//     handler — log it, return safe user message instead."
// ═══════════════════════════════════════════════════════════════════════

@RestControllerAdvice
public class GlobalExceptionHandler {

    // ─── 404 NOT FOUND ──────────────────────────────────────────
    // Custom domain exception → meaningful HTTP response
    @ExceptionHandler(UserNotFoundException.class)
    public ResponseEntity<ErrorResponse> handleUserNotFoundException(UserNotFoundException ex) {
        ErrorResponse error = new ErrorResponse(
                HttpStatus.NOT_FOUND.value(),
                ex.getMessage(),
                System.currentTimeMillis()
        );
        return ResponseEntity.status(HttpStatus.NOT_FOUND).body(error);
    }

    // ─── 400 BAD REQUEST ────────────────────────────────────────
    // Use case: business rule violation (e.g. invalid input value)
    @ExceptionHandler(IllegalArgumentException.class)
    public ResponseEntity<ErrorResponse> handleBadRequest(IllegalArgumentException ex) {
        ErrorResponse error = new ErrorResponse(
                HttpStatus.BAD_REQUEST.value(),
                ex.getMessage(),
                System.currentTimeMillis()
        );
        return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(error);
    }

    // ─── 500 INTERNAL SERVER ERROR (catch-all) ──────────────────
    // Last resort — koi unhandled exception aaye toh user ko 500
    // milega (na ki ugly white-label error page ya stack trace)
    @ExceptionHandler(Exception.class)
    public ResponseEntity<ErrorResponse> handleGeneric(Exception ex) {
        ErrorResponse error = new ErrorResponse(
                HttpStatus.INTERNAL_SERVER_ERROR.value(),
                "Something went wrong: " + ex.getMessage(),
                System.currentTimeMillis()
        );
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(error);
    }
}
