# bits-of-C
A handful of tutorials for creating libraries I've found useful in my own projects.
Some of the tutorials assume a reasonably strong knowledge of C (or programming logic in general). I'm not a professional writer, so if things don't look right or stuff gets a bit wordy, toss an EMAIL my way or post and issue.

All files (including this readme) fall under The Unlicense, meaning you can do whatever you want with anything from this repo.

This repo will be ocassionally updated with documents designed to teach people how to create useful libraries for accelerating their development (if you're stubborn and like to do things from scratch like me!). Everything will be done in C. Sometimes I'll just upload code that's been heavily documented instead of a tutorial document: If you have a reasonably strong understanding of pointers and C in general, you shouldn't have a problem understanding anything that isn't a step-by-step walkthrough.

The final product of each tutorials project will be posted.

If you find discrepancies between tutorials and their accompanied code files, give me a shout and I'll fix it.

## __Coding Conventions__
### Indentation
I indent with tabs and spaces! I primarily work in EMACS, and I'm too lazy to tell it to do otherwise.

### Spacing
The outer-most brackets in an expression or function call are space-padded, but everything else is free game. If it looks good, it looks good.

### Function Names
When I'm writing in C I tend to use underscores in my function names, and it makes for very\_readable\_but\_somewhat\_long function names. I aim for readable code, so this doesn't bother me too much, and auto-completion makes quick work of it either way.

### Struct Names
I use PascalCase when naming structs. It makes them distinguishable from everything else and it's pretty easy to read.

### Variable Names
I'm bad for this. Sometimes I use underscore_separated_words, sometimesItIsCamelCase. I'll try to keep things camelCase for everything in this repo.

### Globals
Globals\_Are\_Like\_This. Every word in the name starts with a capital, and words are separated by underscores. Again, this makes them unique and distinguishable.

### Definitions
Definitions are always ENTIRELY_CAPITALIZED, with each word separated by an underscore. Clear, easy to read, AND_IT_IS_KINDA_FUN_TO_LOOK_AT.