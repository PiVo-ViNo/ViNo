function(fetch_auto_populate name)
    FetchContent_Declare(
        glm	
        URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.7z 
        SOURCE_DIR ${DEPS_SRC}/glm
    )
endfunction()