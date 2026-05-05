package com.arpan.usercrud.exception;

/* ════════════════════════════════════════════════════════════════
 *  📌 CUSTOM EXCEPTION — Domain-specific Error Signal
 * ════════════════════════════════════════════════════════════════
 *  Apni custom exception class banane ka point:
 *  • Code padhne wala turant samjhe — yeh "user not found" wala
 *    case hai, generic Exception nahi
 *  • Specific exception ke liye specific HTTP response (404)
 *  • Service layer simple throw karta hai → handler decide karta
 *    hai HTTP response kya bhejna
 *
 *  ─── extends RuntimeException kyu? Exception kyu nahi? ──────────
 *
 *  Java mein 2 type ke exceptions hain:
 *
 *  1. Checked Exception (extends Exception)
 *     • Compiler force karta hai handle karna ya throws declare
 *     • Example: IOException, SQLException
 *     • Method signature mein "throws" likhna padta hai
 *     • Caller MUST handle (try-catch or rethrow)
 *
 *  2. Unchecked Exception (extends RuntimeException)
 *     • Compiler enforce nahi karta
 *     • Example: NullPointerException, IllegalArgumentException
 *     • Method signature clean rehta hai
 *     • Best for "programming errors" or "exceptional conditions"
 *
 *  Spring philosophy: business exceptions = RuntimeException
 *  Reasons:
 *  • Method signatures saaf rehte hain (throws clutter nahi)
 *  • @Transactional default rollback SIRF RuntimeException pe
 *    hota hai (checked exception pe rollback NAHI)
 *  • Functional interfaces (Supplier, Function) checked allow
 *    nahi karte → RuntimeException seamless lambda mein chalti
 *
 *  ─── super(message) — parent constructor call ───────────────────
 *  RuntimeException ka constructor message accept karta hai.
 *  ex.getMessage() iss message ko return karega — handler ne
 *  iska use kiya hai response mein.
 * ════════════════════════════════════════════════════════════════
 */
public class UserNotFoundException extends RuntimeException {

    public UserNotFoundException(Long id) {
        super("User not found with id: " + id);
    }
}
