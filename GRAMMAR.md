>*script* -> *stmts* **exit**

>*stmts* -> *stmts* \n *stmt* | *stmt* \n 

>*stmt* -> *PERSMEM*<br> 
            | **persona** *PERSONA*<br>
            | **background** *BG*<br>
            | **foreground** *FG*<br>
            | **text** *TEXT* <br>
            | **options** *VAR* **{** *VARS* **}**<br>
            | **if** **(** *VAR* **)** **then** *stmt*<br>
            | **goto** *VAR* <br>
            | *VAR* **:**

>*PERSMEM* -> *VAR* **.** *VAR* **=** *TEXTLINE*

>*PERSONA* -> *VAR* **{** *inside* **}** 

>*inside* -> *N* *type* **=** *TEXTLINE* | *N* *type* **=** *TEXTLINE* **,** *inside*

>*N* -> \n | ∅

*VAR* is used as different FGs, we could access them e.g. Oleg.angry :

>*type* -> **foreground** | **path** | **name** | *VAR* 

    persona Main_hero {path = "D:/photos/", foreground="my_photo.jpeg",
            in_hat = "IMG4389438.png", brave= "kitty.png"}

>*NAME* -> *TEXTLINE*

>*PATH* -> *TEXTLINE*

>*BG* -> **=** *TEXTLINE*

>*FG* -> **=** ( *TEXTLINE* | *VAR* | *VAR* **.** *VAR* )

Text could be with speaker (VAR) or without:
>*TEXT* -> ( *VAR* )( **=** *TEXTLINE* | *TEXTLINE* ) 

    # with speaker
    text John "I love riding my bike! Hell yeah!"
    # without one
    text "In a hole in the ground there lived a hobbit." 

>*TEXTLINE* -> **"** *CH* * **"**

>*VAR* -> *VAR* *CH* | *CH*

>*CH* -> **a** | ... | **z** | **A** | ... | **Z** | **0** | ... | **9** (alnum) 
