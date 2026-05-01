package com.arpan.usercrud.model;

import java.time.Instant;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/* ════════════════════════════════════════════════════════════════════
 *  💾 RefreshToken Entity — DB-stored long-lived token
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Yeh JPA entity refresh tokens DB mein persist karta —
 *  taaki logout pe revoke kar sakein.
 *
 *  ─── 🤔 ACCESS vs REFRESH — Why store only refresh? ───────────────
 *
 *      ┌──────────────────────────┬──────────────────────────────┐
 *      │  Access Token (15 min)   │  Refresh Token (7 days)      │
 *      ├──────────────────────────┼──────────────────────────────┤
 *      │  • Server pe NAHI store   │  • Server pe DB mein STORE   │
 *      │  • Stateless              │  • Stateful (for revocation) │
 *      │  • Signature se validate  │  • DB lookup se validate     │
 *      │  • Har request mein bhejta│  • Sirf /refresh pe bhejta   │
 *      │  • Leak window: 15 min    │  • Leak window: until logout │
 *      └──────────────────────────┴──────────────────────────────┘
 *
 *      Stateful trade-off acceptable hai — refresh token rare use
 *      hota (1-2 baar in 15 min cycle), har API call pe nahi.
 *
 *  ─── 📊 TABLE STRUCTURE ───────────────────────────────────────────
 *
 *      refresh_tokens table:
 *      ┌────┬─────────────┬─────────┬──────────────────────────┐
 *      │ id │  token      │ user_id │  expires_at              │
 *      ├────┼─────────────┼─────────┼──────────────────────────┤
 *      │ 1  │ "RRRR-..."  │   5     │ 2026-05-07T10:30:00Z     │
 *      │ 2  │ "XXXX-..."  │   8     │ 2026-05-07T11:15:00Z     │
 *      └────┴─────────────┴─────────┴──────────────────────────┘
 *
 *  ─── 🎬 LIFECYCLE ─────────────────────────────────────────────────
 *
 *      1️⃣  LOGIN:
 *          AuthController generates UUID → save row in DB
 *
 *      2️⃣  REFRESH (every ~15 min):
 *          findByToken(token) → validate exp → return new accessToken
 *
 *      3️⃣  LOGOUT:
 *          deleteByToken(token) → row removed from DB
 *
 *      4️⃣  EXPIRED:
 *          User offline 7+ days → row expired → next refresh fails
 *          (optional cleanup: scheduled job to delete expired rows)
 *
 *  ─── 🔑 WHY UUID for refresh token (not JWT)? ─────────────────────
 *
 *      Production trade-off:
 *
 *      JWT for refresh:
 *          + Self-validating (no DB lookup needed for signature)
 *          - Can't revoke easily (until expiry)
 *
 *      UUID for refresh (humara approach):
 *          + DB lookup mandatory → instant revocation possible
 *          + Logout pe delete = immediate session end
 *          - DB hit on each refresh (rare, acceptable)
 *
 *      Since refresh used 1x per 15 min, DB lookup overhead is fine.
 *      Revocation guarantee zyada important hai security ke liye.
 *
 *  📐 SOLID:  SRP — Sirf refresh token data + DB mapping
 * ════════════════════════════════════════════════════════════════════
 */
@Entity
@Table(name = "refresh_tokens")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class RefreshToken {

    /*
     *  🔢 Primary key — auto-increment
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /*
     *  🎫 Actual refresh token string (UUID)
     *  unique=true — same token DB mein 2 baar nahi
     */
    @Column(nullable = false, unique = true)
    private String token;

    /*
     *  👤 Owner user ID — kis user ke liye yeh token issued hua
     *  (Foreign key relationship — but simple project mein direct Long)
     *  Production: @ManyToOne with User entity, @JoinColumn use karte
     */
    @Column(nullable = false)
    private Long userId;

    /*
     *  ⏰ Expiration timestamp
     *  Login pe set: Instant.now().plus(7, ChronoUnit.DAYS)
     *  Refresh time pe check: expiresAt.isBefore(Instant.now()) → expired
     */
    @Column(nullable = false)
    private Instant expiresAt;
}
