package com.javatpoint.controller;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import com.javatpoint.model.Claim;
import com.javatpoint.service.ClaimService;

// CONTROLLER: claim CRUD endpoints. Thin -> ClaimService call.
// @PathVariable = URL ka {id} -> arg · @RequestBody = JSON -> Claim · ResponseEntity = body + status.
@RestController
@CrossOrigin(origins = "*")
public class ClaimController {

	@Autowired
	ClaimService claimService;

	// GET: saare claims
	@GetMapping("/api/claims")
	public List<Claim> getAllClaims() {
		return claimService.getAllClaims();
	}

	// GET: us id ke claims (List)
	@GetMapping("/api/claims/{id}")
	public List<Claim> getAllClaimsById(@PathVariable("id") int id) {
		return claimService.getAllClaimsById(id);
	}

	// POST: naya claim create (save)
	@PostMapping("/api/create/claims")
	public void saveClaim(@RequestBody Claim claims) {
		claimService.saveOrUpdateClaim(claims);
	}

	// GET: ek claim (single)
	@GetMapping("/api/get/claim/{id}")
	public Claim getClaimById(@PathVariable("id") int id) {
		return claimService.getClaimaById(id);
	}

	// PUT: update -> na mila to 404, mila to 200 ok(updated)
	@PutMapping("/api/update/claim/{id}")
	public ResponseEntity<Claim> updateClaim(@PathVariable int id, @RequestBody Claim claim) {
		Claim updated = claimService.updateClaim(id, claim);
		if (updated == null) {
			return ResponseEntity.status(HttpStatus.NOT_FOUND).build();
		}
		return ResponseEntity.ok(updated);
	}

	// DELETE: delete -> {"Deleted": true} response
	@DeleteMapping("/api/delete/claim/{id}")
	public ResponseEntity<Map<String, Boolean>> deleteClaim(@PathVariable("id") int id) {
		claimService.deleteClaim(id);
		Map<String, Boolean> response = new HashMap<String, Boolean>();
		response.put("Deleted", Boolean.TRUE);
		return ResponseEntity.ok(response);
	}
}
