package com.javatpoint.controller;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import com.javatpoint.model.Product;
import com.javatpoint.service.ProductService;

import Helper.Helper;

// CONTROLLER: product endpoints -> Excel UPLOAD + list. Thin -> ProductService call.
// @RequestParam("file") MultipartFile = uploaded file (multipart form-data).
@RestController
@CrossOrigin(origins = "*")
public class ProductController {

	@Autowired
	ProductService productService;

	// POST /product/upload: file Excel-format hai? -> service.save (POI parse + bulk-insert) -> ok | 400 (galat format)
	@PostMapping("/product/upload")
	public ResponseEntity<?> upload(@RequestParam("file") MultipartFile file) {
		if (Helper.checkExcelFormat(file)) {
			this.productService.save(file);
			Map<String, String> response = new HashMap<>();
			response.put("message", "File uploaded successfully");
			return ResponseEntity.ok(response);
		} else {
			return ResponseEntity.status(HttpStatus.BAD_REQUEST).body("Please correct the file format.");
		}
	}

	// GET /product: saare products
	@GetMapping("/product")
	public List<Product> getAllProduct() {
		return this.productService.getAllProduct();
	}
}
