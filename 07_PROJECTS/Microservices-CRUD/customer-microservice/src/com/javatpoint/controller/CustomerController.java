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

import com.javatpoint.model.Customers;
import com.javatpoint.service.CustomerService;

//mark class as Controller
@RestController
@CrossOrigin(origins = "*")
public class CustomerController {
	// autowire the BooksService class

	private static final Logger logger = LoggerFactory.getLogger(CustomerController.class);

	@Autowired
	CustomerService customerService;

	@RequestMapping(value = "/api/customers", method = RequestMethod.GET)
	public List<Customers> getAllCustomers() {
		List<Customers> customers = customerService.getAllCustomers();
		logger.info("Successfully retrieved {} customers.", customers.size());
		for (Customers customer : customers) {
			logger.info("Customer details: {}", customer.toString());
		}
		return customers;
	}

	@RequestMapping(value = "/api/customers/{id}", method = RequestMethod.GET)
	public List<Customers> getAllCustomersById(@PathVariable("id") int id) {
		List<Customers> customersById = customerService.getAllCustomersById(id);
		return customersById;
	}

	@RequestMapping(value = "/api/customers/{id}/policies", method = RequestMethod.GET)
	public List<String> getAllCustomersPolicesById(@PathVariable("id") int id) {
		List<Object[]> customersPolicesById = customerService.getAllCustomersPolicesById(id);
		List<String> ans = new ArrayList<String>();
		for (Object[] result : customersPolicesById) {
			Integer customerId = (Integer) result[0];
			String policy = (String) result[1];
			String formatans = "Customer ID: " + customerId + ", Policy: " + policy;
			ans.add(formatans);
		}
		return ans;
	}

	@RequestMapping(value = "/api/customers/{id}/claims", method = RequestMethod.GET)
	public List<String> getAllCustomersClaimsById(@PathVariable("id") int id) {
		List<Object[]> customersClaimsById = customerService.getAllCustomersClaimsById(id);
		List<String> ans = new ArrayList<String>();
		try {
			for (Object[] result : customersClaimsById) {
				Integer customerId = (Integer) result[0];
				String claim = (String) result[1];
				String formatans = "Customer ID: " + customerId + ", Claim: " + claim;
				ans.add(formatans);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ans;
	}

}
