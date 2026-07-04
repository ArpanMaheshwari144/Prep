package com.arpan.orderservice;

public class OrderRequest {
    private String item;
    private double amount;

    public String getItem() { return item; }
    public void setItem(String item) { this.item = item; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }
}
