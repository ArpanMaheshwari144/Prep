package com.arpan.usercrud.exception;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    UserNotFoundException = Custom domain-specific exception
//    Service throws it when user lookup fails
//    GlobalExceptionHandler catches it → 404 response
// ═══════════════════════════════════════════════════════════════════════
//
// USED VIA:
//    repository.findById(id)
//        .orElseThrow(() -> new UserNotFoundException(id));
//
// WHY extends RuntimeException (NOT Exception)?
//
//    Java 2 types of exceptions:
//
//    CHECKED (extends Exception):
//       • Compiler force karta handle ya throws declare
//       • Example: IOException, SQLException
//       • Method signature mein "throws" likhna padta
//       • Caller MUST handle (try-catch or rethrow)
//
//    UNCHECKED (extends RuntimeException):
//       • Compiler enforce nahi karta
//       • Example: NullPointerException, IllegalArgumentException
//       • Method signature clean
//       • Best for programming errors / business exceptions
//
//    SPRING PHILOSOPHY:
//       Business exceptions = RuntimeException
//
//       Reasons:
//          1. Clean method signatures (no throws clutter)
//          2. @Transactional default rollback ONLY on RuntimeException
//             (checked exception pe rollback NAHI hota!)
//          3. Lambda-friendly (Supplier/Function reject checked exceptions)
//
// super(message) — parent constructor call:
//    RuntimeException constructor accepts message
//    ex.getMessage() returns it
//    Handler uses message in response
//
// WHY CUSTOM EXCEPTION (vs Generic)?
//    GENERIC: throw new RuntimeException("User not found");
//             Catch: catch (RuntimeException e) { ... }  ← catches everything
//
//    CUSTOM:  throw new UserNotFoundException(id);
//             Catch: catch (UserNotFoundException e) { ... }  ← specific
//
//    = Self-documenting in stack trace
//    = Specific handler routing in GlobalExceptionHandler
//
// INTERVIEW LINE:
//    "UserNotFoundException extends RuntimeException (unchecked) —
//     Spring's philosophy for business exceptions. Triggers
//     @Transactional rollback automatically and keeps signatures clean.
//     Used with Optional.orElseThrow() for null-safe lookups."
// ═══════════════════════════════════════════════════════════════════════

public class UserNotFoundException extends RuntimeException {

    public UserNotFoundException(Long id) {
        super("User not found with id: " + id);
    }
}
