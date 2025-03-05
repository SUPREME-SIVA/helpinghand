#include <iostream>
#include <list>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <iomanip>

// Example 3: Priority Task Manager
// Demonstrates using a list to manage tasks with different priorities
// and the ability to insert/remove tasks efficiently

// Task class with priority, description, and execution function
class Task {
private:
    int m_id;
    int m_priority;  // Lower number = higher priority
    std::string m_description;
    std::function<void()> m_execute;
    bool m_completed;

public:
    Task(int id, int priority, const std::string& description, std::function<void()> execute)
        : m_id(id), m_priority(priority), m_description(description), m_execute(execute), m_completed(false) {}

    // Getters
    int getId() const { return m_id; }
    int getPriority() const { return m_priority; }
    const std::string& getDescription() const { return m_description; }
    bool isCompleted() const { return m_completed; }

    // Execute the task
    void execute() {
        std::cout << "[Executing] Task #" << m_id << ": " << m_description << std::endl;
        m_execute();
        m_completed = true;
        std::cout << "[Completed] Task #" << m_id << std::endl;
    }

    // Change priority (useful for dynamic prioritization)
    void changePriority(int newPriority) {
        m_priority = newPriority;
    }
};

// Task Manager class using a list to store tasks
class TaskManager {
private:
    std::list<Task> m_tasks;
    int m_nextId;

public:
    TaskManager() : m_nextId(1) {}

    // Add a task with the given priority
    // This method demonstrates the efficiency of list for insertions in the middle
    int addTask(int priority, const std::string& description, std::function<void()> execute) {
        Task newTask(m_nextId++, priority, description, execute);
        
        // Find the right position to insert based on priority
        // This would be inefficient with vectors but is reasonable with lists
        auto it = m_tasks.begin();
        while (it != m_tasks.end() && it->getPriority() <= priority) {
            ++it;
        }
        
        // Insert the task at the correct position - O(1) operation for list
        m_tasks.insert(it, newTask);
        
        return newTask.getId();
    }

    // Remove a task by ID
    bool removeTask(int id) {
        // Find the task with the given ID
        auto it = std::find_if(m_tasks.begin(), m_tasks.end(), 
                               [id](const Task& task) { return task.getId() == id; });
        
        if (it != m_tasks.end()) {
            // Remove the task - O(1) operation for list after finding the element
            m_tasks.erase(it);
            return true;
        }
        
        return false;
    }

    // Change priority of a task
    // This demonstrates the efficiency of moving elements in a list
    bool changePriority(int id, int newPriority) {
        // Find the task
        auto taskIt = std::find_if(m_tasks.begin(), m_tasks.end(), 
                                   [id](const Task& task) { return task.getId() == id; });
        
        if (taskIt == m_tasks.end()) {
            return false;
        }
        
        // Make a copy of the task and update its priority
        Task updatedTask = *taskIt;
        updatedTask.changePriority(newPriority);
        
        // Remove the task from its current position - O(1)
        m_tasks.erase(taskIt);
        
        // Find the new position
        auto insertPos = m_tasks.begin();
        while (insertPos != m_tasks.end() && insertPos->getPriority() <= newPriority) {
            ++insertPos;
        }
        
        // Insert at the new position - O(1)
        m_tasks.insert(insertPos, updatedTask);
        
        return true;
    }

    // Execute the highest priority task
    bool executeHighestPriorityTask() {
        if (m_tasks.empty()) {
            std::cout << "No tasks available." << std::endl;
            return false;
        }
        
        // The highest priority task is at the front
        Task task = m_tasks.front();
        
        // Remove the task - O(1) operation
        m_tasks.pop_front();
        
        // Execute the task
        task.execute();
        
        return true;
    }

    // Display all tasks
    void displayTasks() const {
        if (m_tasks.empty()) {
            std::cout << "No tasks in the queue." << std::endl;
            return;
        }
        
        std::cout << "\n===== Task Queue =====" << std::endl;
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(10) << "Priority" 
                  << "Description" << std::endl;
        std::cout << "----------------------" << std::endl;
        
        for (const auto& task : m_tasks) {
            std::cout << std::left << std::setw(5) << task.getId() 
                      << std::setw(10) << task.getPriority() 
                      << task.getDescription() << std::endl;
        }
        
        std::cout << "======================\n" << std::endl;
    }
};

// Sample task functions
void simpleTask() {
    std::cout << "   Performing simple task..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void complexTask() {
    std::cout << "   Starting complex computation..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "   Complex computation complete!" << std::endl;
}

void ioTask() {
    std::cout << "   Simulating I/O operation..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    std::cout << "   I/O operation complete!" << std::endl;
}

int main() {
    TaskManager taskManager;
    
    // Add some tasks with different priorities
    std::cout << "Adding tasks to the manager..." << std::endl;
    int taskId1 = taskManager.addTask(3, "Generate report", simpleTask);
    int taskId2 = taskManager.addTask(1, "Critical system update", complexTask);
    int taskId3 = taskManager.addTask(2, "Backup database", ioTask);
    int taskId4 = taskManager.addTask(3, "User notification", simpleTask);
    
    // Display initial task queue
    taskManager.displayTasks();
    
    // Change priority of a task
    std::cout << "Changing priority of Task #" << taskId4 << " from 3 to 1" << std::endl;
    taskManager.changePriority(taskId4, 1);
    taskManager.displayTasks();
    
    // Remove a task
    std::cout << "Removing Task #" << taskId1 << std::endl;
    taskManager.removeTask(taskId1);
    taskManager.displayTasks();
    
    // Execute tasks in priority order
    std::cout << "Executing highest priority tasks..." << std::endl;
    while (taskManager.executeHighestPriorityTask()) {
        taskManager.displayTasks();
    }
    
    return 0;
}
