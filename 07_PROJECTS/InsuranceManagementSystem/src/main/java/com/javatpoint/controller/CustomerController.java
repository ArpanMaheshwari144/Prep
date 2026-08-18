package com.javatpoint.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

import com.javatpoint.model.Customers;
import com.javatpoint.service.CustomerService;

// CONTROLLER: customer READ endpoints (sirf GET). Thin -> CustomerService call.
@RestController
@CrossOrigin(origins = "*")
public class CustomerController {

	@Autowired
	CustomerService customerService;

	// GET: saare customers
	@GetMapping("/api/customers")
	public List<Customers> getAllCustomers() {
		return customerService.getAllCustomers();
	}

	// GET: us id ke customer(s)
	@GetMapping("/api/customers/{id}")
	public List<Customers> getAllCustomersById(@PathVariable("id") int id) {
		return customerService.getAllCustomersById(id);
	}

	// GET: us customer ki policies (JOIN -> readable String list)
	@GetMapping("/api/customers/{id}/policies")
	public List<String> getAllCustomersPolicesById(@PathVariable("id") int id) {
		return customerService.getAllCustomersPolicesById(id);
	}

	// GET: us customer ke claims (JOIN -> readable String list)
	@GetMapping("/api/customers/{id}/claims")
	public List<String> getAllCustomersClaimsById(@PathVariable("id") int id) {
		return customerService.getAllCustomersClaimsById(id);
	}
}
