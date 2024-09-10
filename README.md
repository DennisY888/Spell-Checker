# Spelling Verifier
### Skills
- C, memory allocation, data structures (hash table and linked list), performance check
### Technical Considerations
- Dynamic resizing (resembling python dictionaries) - when load factor > 0.75, size of hash table doubles, minimizing collisions/chaining 
- Implementing data structures from scratch 
- Visualize difference in performance between variations of hash table implementations (different sizes, hashing)
- Memory management 
### How this works
- Load a dictionary of words into memory (hash table for quick lookup). 
- Read a text file and check each word to see if it exists in the dictionary. 
- Report any misspelled words and track performance metrics (how long different tasks took).
### Usage in terminal
- “make speller” 
- “./speller [text]”
### Metrics
WORDS IN DICTIONARY:  143091  
WORDS IN TEXT:        904612  
TIME IN TOTAL(s):        0.39
