package com.javatpoint.service;

import java.util.List;

import javax.persistence.EntityNotFoundException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.javatpoint.model.Claim;
import com.javatpoint.model.Policy;
import com.javatpoint.repository.ClaimRepository;

// SERVICE (business-logic layer): controller <-> repository ke beech. claim ka CRUD + rules.
@Service
public class ClaimService {
	@Autowired
	ClaimRepository claimRepository;   // DB access inject

	public List<Claim> getAllClaims() {
		return claimRepository.getAllClaims();
	}
	public List<Claim> getAllClaimsById(int id) {
		return claimRepository.getAllClaimsById(id);
	}
	// JPA save() = id nayi -> INSERT, purani -> UPDATE (ek method dono)
	public void saveOrUpdateClaim(Claim claims) {
		claimRepository.save(claims);
	}

	public Claim getClaimaById( int id) {
		return claimRepository.getClaimsById(id);
	}

	// PARTIAL update: pehle fetch -> null-check -> sirf claims field badlo -> save
	public Claim updateClaim(int id, Claim claim) {
		Claim c = claimRepository.getClaimsById(id);
		if (c == null) {
			return null;
		}
		c.setClaims(claim.getClaims());
		return claimRepository.save(c);
	}

	// delete: na mile to EntityNotFoundException (silently-ignore nahi) -> phir delete
	public void deleteClaim(int id) {
		Claim c = claimRepository.findById(id)
				.orElseThrow(() -> new EntityNotFoundException("Claim Not Found " + id));
		claimRepository.delete(c);
	}

}
