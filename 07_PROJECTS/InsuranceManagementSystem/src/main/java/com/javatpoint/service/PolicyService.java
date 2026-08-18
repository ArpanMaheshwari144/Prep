package com.javatpoint.service;

import java.util.List;

import javax.persistence.EntityNotFoundException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.javatpoint.model.Policy;
import com.javatpoint.repository.PolicyRepository;

// SERVICE: policy ka CRUD + rules (ClaimService jaisa pattern, sirf entity Policy).
@Service
public class PolicyService {

	@Autowired
	PolicyRepository policyRepository;

	public List<Policy> getAllPolices() {
		return policyRepository.getAllPolices();
	}

	public List<Policy> getAllPoliciesById(int id) {
		return policyRepository.getAllPoliciesById(id);
	}
	// JPA save() = id nayi -> INSERT, purani -> UPDATE
	public void saveOrUpdatePolicy(Policy policy) {
		policyRepository.save(policy);
	}

	public Policy getPoliciesById(int id) {
		return policyRepository.getPoliciesById(id);
	}

	// PARTIAL update: fetch -> null-check -> sirf policies field badlo -> save
	public Policy updatePolicy(int id, Policy policy) {
		Policy p = policyRepository.getPoliciesById(id);
		if (p == null) {
			return null;
		}
		p.setPolicies(policy.getPolicies());
		return policyRepository.save(p);
	}

	// delete: na mile to EntityNotFoundException -> phir delete
	public void deletePolicy(int id) {
		Policy p = policyRepository.findById(id)
				.orElseThrow(() -> new EntityNotFoundException("Policy Not Found " + id));
		policyRepository.delete(p);
	}

}
