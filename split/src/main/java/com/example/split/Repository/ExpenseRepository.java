package com.example.split.Repository;

import com.example.split.Entities.Expense;
import com.example.split.Entities.Group;
import org.springframework.data.jpa.repository.JpaRepository;
import java.util.List;

public interface ExpenseRepository extends JpaRepository<Expense, Long> {
    List<Expense> findByGroup(Group group);
    List<Expense> findByGroupOrderByDateDesc(Group group);
}