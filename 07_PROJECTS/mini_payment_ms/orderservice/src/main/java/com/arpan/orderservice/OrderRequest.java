package com.arpan.orderservice;

// OrderRequest — POST /order ka request body { item, amount } (DTO). @RequestBody isme JSON bharta.
public class OrderRequest {
    private String item;
    private double amount;

    public String getItem() { return item; }
    public void setItem(String item) { this.item = item; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }
}
