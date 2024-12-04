#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

// Constants for simulation
const int PAGE_SIZE = 4;                         // Each page is 4 KB
const int MEMORY_SIZE = 64;                      // Total memory size (in KB)
const int TOTAL_PAGES = MEMORY_SIZE / PAGE_SIZE; // Number of pages in memory

class VirtualMemoryManager
{
public:
    VirtualMemoryManager()
    {
        // Initialize all pages as free (0 means free, 1 means allocated)
        memory.resize(TOTAL_PAGES, 0);
    }

    // Allocate a process (simulated as pages)
    bool allocate(int processId, int numPages)
    {
        if (numPages > TOTAL_PAGES)
        {
            std::cout << "Error: Not enough memory available." << std::endl;
            return false;
        }

        int startPage = -1;
        int contiguousFreePages = 0;

        // Try to find contiguous free pages
        for (int i = 0; i < TOTAL_PAGES; ++i)
        {
            if (memory[i] == 0)
            {
                if (startPage == -1)
                {
                    startPage = i; // Start of free pages
                }
                ++contiguousFreePages;

                // If we found enough contiguous free pages, allocate
                if (contiguousFreePages == numPages)
                {
                    for (int j = startPage; j < startPage + numPages; ++j)
                    {
                        memory[j] = 1; // Allocate pages
                    }
                    allocations[processId] = {startPage, numPages};
                    std::cout << "Process " << processId << " allocated " << numPages << " pages at starting page " << startPage << "." << std::endl;
                    return true;
                }
            }
            else
            {
                startPage = -1; // Reset if memory is fragmented
                contiguousFreePages = 0;
            }
        }

        // If we reach here, no sufficient contiguous block of pages was found
        std::cout << "Error: Not enough contiguous memory available for process " << processId << "." << std::endl;
        return false;
    }

    // Deallocate memory for a given process
    void deallocate(int processId)
    {
        if (allocations.find(processId) != allocations.end())
        {
            int startPage = allocations[processId].first;
            int numPages = allocations[processId].second;

            for (int i = startPage; i < startPage + numPages; ++i)
            {
                memory[i] = 0; // Free the pages
            }

            allocations.erase(processId); // Remove process allocation info
            std::cout << "Process " << processId << " deallocated from pages " << startPage << " to " << startPage + numPages - 1 << "." << std::endl;
        }
        else
        {
            std::cout << "Error: Process " << processId << " not found." << std::endl;
        }
    }

    // Print the current memory status (0 = free, 1 = allocated)
    void printMemoryStatus()
    {
        std::cout << "Memory Status: ";
        for (int i = 0; i < TOTAL_PAGES; ++i)
        {
            std::cout << memory[i] << " ";
        }
        std::cout << std::endl;
    }

    // Print the allocations of all processes
    void printAllocations()
    {
        std::cout << "Current Allocations: \n";
        for (auto &allocation : allocations)
        {
            std::cout << "Process " << allocation.first << " -> Pages " << allocation.second.first << " to " << allocation.second.first + allocation.second.second - 1 << std::endl;
        }
    }

private:
    std::vector<int> memory;                        // Simulated memory (0 = free, 1 = allocated)
    std::map<int, std::pair<int, int>> allocations; // Process ID -> (start page, num pages)
};

int main()
{
    VirtualMemoryManager vmm;

    vmm.printMemoryStatus(); // Initial memory status

    // Allocate memory for processes
    vmm.allocate(1, 4); // Allocate 4 pages for process 1
    vmm.allocate(2, 2); // Allocate 2 pages for process 2
    vmm.allocate(3, 3); // Allocate 3 pages for process 3

    vmm.printAllocations();  // Print all allocations
    vmm.printMemoryStatus(); // Memory status after allocations

    // Deallocate memory
    vmm.deallocate(2); // Deallocate process 2
    vmm.deallocate(1); // Deallocate process 1

    vmm.printAllocations();  // Print remaining allocations
    vmm.printMemoryStatus(); // Memory status after deallocations

    return 0;
}
