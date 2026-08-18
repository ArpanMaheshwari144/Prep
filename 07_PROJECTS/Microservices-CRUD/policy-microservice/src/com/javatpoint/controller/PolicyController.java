package com.javatpoint.controller;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.persistence.EntityNotFoundException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import com.javatpoint.model.Policy;
import com.javatpoint.repository.PolicyRepository;
import com.javatpoint.service.PolicyService;


//mark class as Controller
@RestController
@CrossOrigin(origins="*")
public class PolicyController {
//autowire the BooksService class
	
	private static final Logger logger=LoggerFactory.getLogger(PolicyController.class);
	
	
	
	
	
	@Autowired
	PolicyService policyService;
	
	@Autowired
	PolicyRepository policyRepository;
	
	
	
	
	
    @RequestMapping(value = "/api/policies",method = RequestMethod.GET)
    public List<Policy> getAllPolicies(){
    	List<Policy> policies = new ArrayList<Policy>();
    	 try {
			policies = policyService.getAllPolices();
		} catch (Exception e) {
			e.printStackTrace();
		}
    	 return policies;
    }
    @RequestMapping(value = "/api/policies/{id}",method = RequestMethod.GET)
    public List<Policy> getAllPoliciesById(@PathVariable("id")int id){
    	List<Policy> policiesById=new ArrayList<Policy>();
    	try {
    		policiesById=policyService.getAllPoliciesById(id);
    	}catch (Exception e) {
    		e.printStackTrace();
    	}
    	return policiesById;
    }
    @RequestMapping(value="/api/create/policies",method = RequestMethod.POST)
	private void savePolicies(@RequestBody Policy policy) {
    	policyService.saveOrUpdatePolicy(policy);	
	}
   
    @PutMapping(value="/api/update/policy/{id}")
    public ResponseEntity<Policy> updatePolicy(@PathVariable int id,@RequestBody Policy policy){
    	Policy p = policyService.getPoliciesById(id);
    	if(p==null) {
    		return ResponseEntity.status(HttpStatus.NOT_FOUND).build();
    	}
    	p.setPolicies(policy.getPolicies());
    	Policy updatePolicy = policyRepository.save(p);
    	return ResponseEntity.ok(updatePolicy);	
    }
    @RequestMapping(value = "/api/get/policy/{id}",method = RequestMethod.GET)
    public Policy getPolicyById(@PathVariable("id")int id){
    	 Policy policyById = policyService.getPoliciesById(id);
    	 return policyById;
    }
    
    
    @RequestMapping(value = "/api/policies/allnames",method = RequestMethod.GET)
    public List<String> getAllPoliciesName(){
    	List<String> policies = new ArrayList<String>();
    	 try {
			policies = policyService.getAllPolicesName();
		} catch (Exception e) {
			e.printStackTrace();
		}
    	 return policies;
    }
    
    
}
