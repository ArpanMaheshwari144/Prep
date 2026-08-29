package com.javatpoint.service;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.javatpoint.model.Customers;
import com.javatpoint.repository.CustomerRepository;


// SERVICE: customer logic. Khaas kaam = repo ke JOIN-result (List<Object[]>) ko readable String me todna.
@Service
public class CustomerService {

	@Autowired
	CustomerRepository customerRepository;

	public List<Customers> getAllCustomers() {
		return customerRepository.getAllCustomers();
	}

	public List<Customers> getAllCustomersById(int id) {
		return customerRepository.getAllCustomersById(id);
	}

	// repo ne JOIN ka kachcha List<Object[]> diya -> har row todo: r[0]=id, r[1]=policy -> readable String banao
	public List<String> getAllCustomersPolicesById(int id) {
		List<Object[]> rows = customerRepository.getAllCustomersPolicesById(id);
		List<String> ans = new ArrayList<String>();
		for (Object[] r : rows) {
			Integer customerId = (Integer) r[0];
			String policy = (String) r[1];
			ans.add("Customer ID: " + customerId + ", Policy: " + policy);
		}
		return ans;
	}

	// wahi format, claim ke saath. try-catch -> claim-side exception pe crash nahi, khaali list wapas.
	public List<String> getAllCustomersClaimsById(int id) {
		List<String> ans = new ArrayList<String>();
		try {
			List<Object[]> rows = customerRepository.getAllCustomersClaimsById(id);
			for (Object[] r : rows) {
				Integer customerId = (Integer) r[0];
				String claim = (String) r[1];
				ans.add("Customer ID: " + customerId + ", Claim: " + claim);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ans;
	}


}
