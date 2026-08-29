package com.arpan.todoapp.dto;
// File: src/main/java/com/arpan/todoapp/dto/UserResponse.java

import com.arpan.todoapp.model.User;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// UserResponse = RESPONSE DTO — User ka SAFE version (password HATA ke bhejne ko)
//    → API response me password (hash) kabhi na jaaye; entity internal-only.
//
// ═══ STATIC FACTORY METHOD: from(User) ═══
//    public static UserResponse from(User user)
//
//    ★ static kyun → object banaye BINA call (UserResponse.from(user));
//                    hum UserResponse BANA rahe → pehle se ek hai hi nahi.
//    ★ return type = UserResponse → method ka kaam hi naya UserResponse dena.
//    ★ mapping EK jagah (DRY) → controller me bas UserResponse.from(user).
//
//    ★ "STATIC FACTORY METHOD" = Java standard idiom (aam-taur pe dikhta):
//          Integer.valueOf(5)     List.of(1, 2, 3)     Optional.of(x)
//          LocalDate.now()        String.valueOf(10)   Collections.emptyList()
//       → object banane ka clean/named tareeka ("new" ke bajaye).
//
//    ★ NOTE — ye from() static-factory is project me PEHLI BAAR (baaki classes me nahi tha).
//       kaam: entity → DTO mapping ek jagah (DRY).
//
//    ★ ABHI STATUS: DTO bana hai PAR controller abhi bhi ENTITY seedha return karta:
//         ABHI (controller):   ResponseEntity.ok(service.getById(id))
//                              → User entity jaata (password bhi) = LEAK
//         DTO USE karne pe:    ResponseEntity.ok(UserResponse.from(service.getById(id)))
//                              → from() se map → sirf id/name/email (password strip)
//       from() TAB call hoga jab controller ko ye change karenge (abhi PENDING).
// ═══════════════════════════════════════════════════════════════════════

@Data
@NoArgsConstructor
@AllArgsConstructor
public class UserResponse {

    // id/name/email ONLY — password JAAN-BOOJH ke nahi (yehi DTO ka maqsad)
    private Long id;
    private String name;
    private String email;

    // static factory: User entity → UserResponse (password chhoda)
    public static UserResponse from(User user) {
        return new UserResponse(user.getId(), user.getName(), user.getEmail());
    }
}
