package com.javatpoint.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.javatpoint.model.Policy;
import com.javatpoint.repository.PolicyRepository;
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
	public void saveOrUpdatePolicy(Policy policy) {
		policyRepository.save(policy);
	}
	
	public Policy getPoliciesById(int id) {
		return policyRepository.getPoliciesById(id);
	}
	
	public List<String> getAllPolicesName() {
		return policyRepository.getAllPolicesName();
	}

	

}
