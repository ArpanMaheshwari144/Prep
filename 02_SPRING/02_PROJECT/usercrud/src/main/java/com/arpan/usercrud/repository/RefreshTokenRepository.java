package com.arpan.usercrud.repository;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.arpan.usercrud.model.RefreshToken;

/* ════════════════════════════════════════════════════════════════════
 *  💾 RefreshTokenRepository — DB access for refresh tokens
 * ════════════════════════════════════════════════════════════════════
 *
 *  📦 Spring Data JPA interface — DB CRUD operations on
 *  refresh_tokens table.
 *
 *  ─── 🪄 SPRING DATA JPA MAGIC ──────────────────────────────────────
 *
 *      Hum sirf method NAAM likhte — Spring runtime pe SQL bana deta!
 *
 *      Method: findByToken(String token)
 *      Spring → SELECT * FROM refresh_tokens WHERE token = ?
 *
 *      Method: deleteByToken(String token)
 *      Spring → DELETE FROM refresh_tokens WHERE token = ?
 *
 *      No SQL writing, no Hibernate boilerplate — pure magic.
 *      JpaRepository extend karne se inherit hota:
 *      • save()
 *      • findById()
 *      • findAll()
 *      • count()
 *      • deleteAll()
 *      ... etc.
 *
 *  ─── 🎬 USE CASES (kahan kya use hota) ────────────────────────────
 *
 *      LOGIN:
 *          refreshTokenRepo.save(newRefreshToken)
 *          → INSERT row in DB
 *
 *      REFRESH:
 *          Optional<RefreshToken> stored = refreshTokenRepo.findByToken(token);
 *          if (stored.isPresent() && !expired) → naya access issue
 *
 *      LOGOUT:
 *          refreshTokenRepo.deleteByToken(token)
 *          → DELETE row from DB
 *
 *  ─── ⚠️ @Transactional on deleteByToken — kyu? ────────────────────
 *
 *      Spring Data JPA derived DELETE methods @Transactional chahiye
 *      caller side pe (default JpaRepository methods like delete()
 *      auto-transactional, but custom derived NAHI).
 *
 *      Hum yahan method-level @Transactional laga rahe — alternative
 *      to caller (AuthController) pe lagane ka.
 * ════════════════════════════════════════════════════════════════════
 */
@Repository
public interface RefreshTokenRepository extends JpaRepository<RefreshToken, Long> {

    /*
     *  🔍 Find token by string value
     *  Used by: AuthController.refresh() + logout()
     *  Returns: Optional — present if token exists, empty if not
     */
    Optional<RefreshToken> findByToken(String token);

    /*
     *  🗑️ Delete by token string (used in logout)
     *  @Transactional mandatory for derived delete methods
     *  Returns void — silently completes (or no-op if not found)
     */
    @Transactional
    void deleteByToken(String token);
}
