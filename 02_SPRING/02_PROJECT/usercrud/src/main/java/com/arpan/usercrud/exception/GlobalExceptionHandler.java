package com.arpan.usercrud.exception;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

import com.arpan.usercrud.dto.ErrorResponse;

/* ════════════════════════════════════════════════════════════════
 *  📌 GLOBAL EXCEPTION HANDLER — Centralized Error Handling
 * ════════════════════════════════════════════════════════════════
 *  Yeh class poore application ke saare controllers mein throw
 *  hone wali exceptions ko CENTRALIZED tarike se handle karti hai.
 *
 *  Without this: har controller method mein try-catch likhna padta
 *  With this:    controller saaf rehta, exceptions yahaan flow ho
 *                jati hain, consistent JSON response milta
 *
 *  ─── @RestControllerAdvice — interview gold ─────────────────────
 *
 *  Yeh = @ControllerAdvice + @ResponseBody
 *
 *  • @ControllerAdvice → "Yeh class saare controllers ke liye
 *                         shared advice (cross-cutting concerns)
 *                         provide karti hai"
 *  • @ResponseBody     → "Methods ka return value direct JSON banao"
 *
 *  Bina @ResponseBody (sirf @ControllerAdvice) toh ErrorResponse
 *  ko view-name samjhega Spring → 500 error.
 *
 *  ─── @ExceptionHandler(SomeException.class) ─────────────────────
 *
 *  "Jab koi controller SomeException throw kare → yeh method call
 *  karo response banane ke liye"
 *
 *  Spring sabse SPECIFIC handler dhundhta hai. UserNotFoundException
 *  throw hua → UserNotFoundException ka handler chalega (Exception
 *  ka generic handler ignore hoga). Yeh polymorphism + reflection
 *  ka use hai.
 *
 *  ─── HANDLER PRIORITY ORDER (specific → generic) ────────────────
 *
 *      UserNotFoundException  →  404 (most specific)
 *             │
 *      IllegalArgumentException → 400
 *             │
 *      Exception              →  500 (catch-all fallback)
 *
 *  ─── HTTP STATUS CHEAT SHEET ────────────────────────────────────
 *  200 OK            → success with body
 *  201 Created       → POST success (resource banaya)
 *  204 No Content    → DELETE success
 *  400 Bad Request   → client galti (invalid data)
 *  401 Unauthorized  → login chahiye
 *  403 Forbidden     → login hai par permission nahi
 *  404 Not Found     → resource missing
 *  500 Server Error  → server side bug / unhandled exception
 *
 *  ─── PRODUCTION TIP (interview bonus) ───────────────────────────
 *  Real apps mein generic Exception handler mein ex.getMessage()
 *  client ko nahi bhejna chahiye — internal stack trace leak ho
 *  sakti hai. Generic message bhejo + actual exception logger se
 *  log karo. Yahaan brushup ke liye direct rakha hai.
 * ════════════════════════════════════════════════════════════════
 */
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
