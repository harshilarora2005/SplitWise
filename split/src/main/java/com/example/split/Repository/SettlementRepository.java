package com.example.split.Repository;

import com.example.split.Entities.Group;
import com.example.split.Entities.Settlement;
import org.springframework.data.jpa.repository.JpaRepository;
import java.util.List;

public interface SettlementRepository extends JpaRepository<Settlement, Long> {
    List<Settlement> findByGroup(Group group);
    List<Settlement> findByGroupAndSettled(Group group, boolean settled);
}