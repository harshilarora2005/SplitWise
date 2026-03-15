package com.example.split.Repository;

import com.example.split.Entities.Group;
import com.example.split.Entities.User;
import org.springframework.data.jpa.repository.JpaRepository;
import java.util.List;

public interface GroupRepository extends JpaRepository<Group, Long> {
    List<Group> findByMembersContaining(User user);
}