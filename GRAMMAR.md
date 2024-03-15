*script* -> *stmts* **exit**

*stmts* -> *stmts* \n *stmt* | *stmt* \n 

*stmt* -> *PERSMEM*<br> 
            | **persona** *PERSONA*<br>
            | **background** *BG*<br>
            | **foreground** *FG*<br>
            | **text** *TEXT* <br>
            | **options** *VAR* **{** *VARS* **}**<br>
            | **if** **(** *VAR* **)** **then** *stmt*<br>
            | **goto** *VAR* <br>
            | *VAR* **:**

*PERSMEM* -> *VAR* **.** *VAR* **=** *TEXTLINE*

*PERSONA* -> *VAR* **{** *inside* **}** 

*inside* -> *N* *type* **=** *TEXTLINE* | *N* *type* **=** *TEXTLINE* **,** *inside*

*N* -> \n | ∅

*VAR* is used as different FGs, we could access them e.g. Oleg.angry :

*type* -> **foreground** | **path** | **name** | *VAR* 

*NAME* -> *TEXTLINE*

*PATH* -> *TEXTLINE*

*BG* -> **=** *TEXTLINE*

*FG* -> **=** ( *TEXTLINE* | *VAR* | *VAR* **.** *VAR* )

*TEXT* -> **=** *TEXTLINE* | *TEXTLINE* 

*TEXTLINE* -> **"** *CH* **"** | **""**

*VAR* -> *VAR* *CH* | *CH*

*CH* -> **a** | ... | **z** | **A** | ... | **Z** | **0** | ... | **9** (alnum) 
