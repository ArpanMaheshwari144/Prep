package com.example.practice.exception;

/*
 * ============================================================
 * YE KYA HAI: Global Exception Handler — kahi bhi error aaye, YAHAN handle. Clean JSON error response.
 * KYUN CHAHIYE: Bina iske client ko ugly stack trace dikhega — 50 lines ka Java error.
 * KYA KARTA: Exception catch → proper JSON response + correct HTTP status code.
 *
 * NAHI HUA TO: Client ko dikhega:
 *   "java.lang.RuntimeException: Student not found at com.example.practice.service.StudentService.getStudentById(StudentService.java:49)
 *    at sun.reflect.NativeMethodAccessorImpl..."  <-- UGLY + SECURITY RISK (internal paths leak)
 *
 * REAL WORLD ANALOGY:
 *   App ek FACTORY hai. Machines (controllers/services) mein kabhi error aata.
 *   BINA GlobalExceptionHandler: Error ka alarm seedha customer (client) tak jaata — PANIC.
 *     Customer ko dikhega: "Machine #3, Line 49, Bolt #7 toota" — customer ko kya matlab?
 *   WITH GlobalExceptionHandler: Beech mein ek MANAGER baitha — error pakde, clean message de.
 *     Customer ko dikhega: "Sorry, aapka product abhi available nahi hai" — PROFESSIONAL.
 *
 * FLOW:
 *   Kahi bhi exception throw hua →
 *   Spring dekhta: @RestControllerAdvice hai? → HAI →
 *   Exception type match karta → RuntimeException? → handleRuntime() call →
 *   Clean JSON response bhejta → client ko milta: { "error": "Student not found with id: 99" }
 * ============================================================
 */

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.*;
import java.util.*;

/*
 * ============================================================
 * @RestControllerAdvice
 *
 * YE KYA HAI: Global advice for ALL controllers. Exception handling ek jagah.
 *   = @ControllerAdvice + @ResponseBody (2-in-1)
 *
 * KYUN LAGAYA:
 *   - Har controller mein try-catch likhna = duplicate code = DRY violation
 *   - Ek jagah handle = clean, maintainable, consistent error responses
 *   - Sab controllers ke exceptions AUTOMATICALLY yahan route ho jaate
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Har controller mein try-catch likhna padega — 6 controllers = 6 jagah same code
 *   - Ya ugly stack trace client ko dikhega — internal paths, class names LEAK
 *   - Inconsistent errors — koi controller JSON de, koi HTML, koi stack trace
 *   - SECURITY RISK — attacker ko internal structure pata chal jayega
 *
 * REAL WORLD ANALOGY:
 *   @RestControllerAdvice = CIRCUS ka SAFETY NET.
 *   Trapeze artist (controller) gire → net (handler) pakde → safely land kare.
 *   Bina net: seedha zameen pe (ugly error client tak) → audience SHOCKED.
 *
 *   Doosri analogy:
 *   @RestControllerAdvice = COMPANY ka PR DEPARTMENT.
 *   Company mein kuch galat hua → PR department CLEAN statement de public ko.
 *   Bina PR: engineer khud press conference de → "Code mein line 49 pe NullPointer aaya" → public confused.
 *
 * @RestControllerAdvice vs @ControllerAdvice:
 *   @ControllerAdvice = HTML error pages return (Thymeleaf/JSP)
 *   @RestControllerAdvice = JSON error response return (REST API) — humein ye chahiye
 * ============================================================
 */
@RestControllerAdvice
public class GlobalExceptionHandler {

    // ==========================================
    // RuntimeException — student not found, generic errors
    // ==========================================

    /*
     * ============================================================
     * @ExceptionHandler(RuntimeException.class)
     *
     * YE KYA HAI: Ye method TAB chalega jab RuntimeException throw ho kahi bhi app mein.
     *
     * KYUN LAGAYA: Service mein throw new RuntimeException("Student not found") kiya →
     *   ye method catch karega → clean JSON response banayega → client ko bhejega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Spring DEFAULT error handler chalega
     *   - Client ko milega: { "timestamp": "...", "status": 500, "error": "Internal Server Error",
     *     "path": "/api/students/99", "trace": "java.lang.RuntimeException at..." }
     *   - Stack trace VISIBLE — security risk + ugly
     *
     * REAL WORLD ANALOGY:
     *   @ExceptionHandler = CUSTOMER SERVICE ka specific department.
     *   "Order missing?" → Order department handle karega (RuntimeException handler).
     *   "Payment failed?" → Payment department handle karega (PaymentException handler).
     *   Spring CLOSEST MATCH dhundhta — specific handler pehle, generic baad mein.
     *
     * SPRING MATCHING:
     *   - RuntimeException throw hua → @ExceptionHandler(RuntimeException.class) mila → ye chale
     *   - MethodArgumentNotValidException throw hua → uska specific handler chale
     *   - Specific handler nahi mila → parent class ka handler dhundhe
     * ============================================================
     */
    @ExceptionHandler(RuntimeException.class)
    public ResponseEntity<Map<String, String>> handleRuntime(RuntimeException ex) {
        Map<String, String> error = new HashMap<>();
        error.put("error", ex.getMessage());
        error.put("type", ex.getClass().getName());
        if (ex.getCause() != null) error.put("cause", ex.getCause().getMessage());
        // ex.getMessage() = "Student not found with id: 99" (Service mein jo likha tha)
        return new ResponseEntity<>(error, HttpStatus.NOT_FOUND);  // 404
        /*
         * CLIENT KO MILEGA:
         *   Status: 404 Not Found
         *   Body: { "error": "Student not found with id: 99", "type": "java.lang.RuntimeException" }
         *   CLEAN. Professional. Secure (no stack trace, no internal paths).
         */
    }

    // ==========================================
    // Validation errors — @Valid fail
    // ==========================================

    /*
     * ============================================================
     * @ExceptionHandler(MethodArgumentNotValidException.class)
     *
     * YE KYA HAI: Ye exception TAB aata jab @Valid fail ho — validation errors ke liye.
     *
     * KYUN LAGAYA: Client ko EXACT batana ki kaunsa field galat hai aur kyun.
     *   Generic "Validation failed" se koi samjh nahi aata.
     *   Specific field errors se client ko pata chalta kya fix karna hai.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Client ko generic "Validation failed" milega — kaunsa field galat? PATA NAHI.
     *   - Frontend form validation impossible — "name galat hai ya email?" dunno.
     *   - User frustrated — form dobara dobara submit kare, har baar "failed"
     *
     * REAL WORLD ANALOGY:
     *   BINA handler: Teacher bole "Answer galat hai" — kaunsa? pata nahi.
     *   WITH handler: Teacher bole "Q3 galat, Q7 galat — Q3 mein formula wrong, Q7 mein calculation wrong."
     *   Student (client) ko EXACT pata kya fix karna hai.
     *
     * FLOW:
     *   Client ne blank name + invalid email bheja →
     *   @Valid fail → MethodArgumentNotValidException throw →
     *   Ye handler catch karta → har field ki error nikalta → JSON mein bhejta
     * ============================================================
     */
    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<Map<String, String>> handleValidation(MethodArgumentNotValidException ex) {
        Map<String, String> errors = new HashMap<>();
        ex.getBindingResult().getFieldErrors().forEach(err ->
            errors.put(err.getField(), err.getDefaultMessage())
            /*
             * getBindingResult() → validation result (kya kya fail hua)
             * getFieldErrors() → List of FieldError (har galat field)
             * getField() → "name", "email", "age" (field ka naam)
             * getDefaultMessage() → "Name is required" (annotation mein jo likha)
             *
             * Loop karke har field ki error Map mein daalta:
             *   { "name" → "Name is required",
             *     "email" → "Invalid email",
             *     "age" → "Age must be at least 18" }
             */
        );
        return new ResponseEntity<>(errors, HttpStatus.BAD_REQUEST);  // 400
        /*
         * CLIENT KO MILEGA:
         *   Status: 400 Bad Request
         *   Body: {
         *     "name": "Name is required",
         *     "email": "Invalid email",
         *     "age": "Age must be at least 18"
         *   }
         *   Har field ki SPECIFIC error — client/frontend ko pata kya fix karna hai.
         */
    }
}
