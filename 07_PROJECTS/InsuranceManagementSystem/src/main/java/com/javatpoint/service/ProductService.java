package com.javatpoint.service;

import java.io.IOException;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import com.javatpoint.model.Product;
import com.javatpoint.repository.ProductRepo;

import Helper.Helper;


// SERVICE: product ka Excel-UPLOAD (Apache POI). uploaded file padho -> List<Product> -> DB me bulk-save.
@Service
public class ProductService {
	@Autowired
	private ProductRepo productRepo;

	// MultipartFile = uploaded file. Helper (POI) Excel ko List<Product> me convert -> saveAll (batch insert)
	public void save(MultipartFile file) {

		try {
		     List<Product> product =Helper.convertToListExcelOfProduct(file.getInputStream());
		     this.productRepo.saveAll(product);
		}catch (IOException e) {                 // file padhne me dikkat -> crash nahi
			e.printStackTrace();

		}


	}

	public List<Product>getAllProduct(){
		return this.productRepo.findAll();       // saare products (JpaRepo built-in)


	}

}
