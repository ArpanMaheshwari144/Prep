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

import com.javatpoint.model.Policy;
import com.javatpoint.service.PolicyService;

// CONTROLLER: policy CRUD endpoints (ClaimController jaisa pattern). Thin -> PolicyService call.
@RestController
@CrossOrigin(origins = "*")
public class PolicyController {

	@Autowired
	PolicyService policyService;

	// GET: saari policies
	@GetMapping("/api/policies")
	public List<Policy> getAllPolicies() {
		return policyService.getAllPolices();
	}

	// GET: us id ki policies (List)
	@GetMapping("/api/policies/{id}")
	public List<Policy> getAllPoliciesById(@PathVariable("id") int id) {
		return policyService.getAllPoliciesById(id);
	}

	// POST: nayi policy create (save)
	@PostMapping("/api/create/policies")
	public void savePolicies(@RequestBody Policy policy) {
		policyService.saveOrUpdatePolicy(policy);
	}

	// GET: ek policy (single)
	@GetMapping("/api/get/policy/{id}")
	public Policy getPolicyById(@PathVariable("id") int id) {
		return policyService.getPoliciesById(id);
	}

	// PUT: update -> na mila to 404, mila to 200 ok(updated)
	@PutMapping("/api/update/policy/{id}")
	public ResponseEntity<Policy> updatePolicy(@PathVariable int id, @RequestBody Policy policy) {
		Policy updated = policyService.updatePolicy(id, policy);
		if (updated == null) {
			return ResponseEntity.status(HttpStatus.NOT_FOUND).build();
		}
		return ResponseEntity.ok(updated);
	}

	// DELETE: delete -> {"Deleted": true} response
	@DeleteMapping("/api/delete/policy/{id}")
	public ResponseEntity<Map<String, Boolean>> deletePolicy(@PathVariable("id") int id) {
		policyService.deletePolicy(id);
		Map<String, Boolean> response = new HashMap<String, Boolean>();
		response.put("Deleted", Boolean.TRUE);
		return ResponseEntity.ok(response);
	}
}
