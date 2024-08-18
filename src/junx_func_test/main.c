#include <stdio.h>
#include "junx/core/container/junx_vector.h"
#include "junx/core/rtti/junx_object.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void process_junx_objct(junx_object *  obj)
{


     printf("obj is %u\n", JUNX_METHODS(obj)->_type_id());
    
}

int print_dir_content(const char* dir_name, int level)
{
    int res = 0;
    char buffer[PATH_MAX] = {0};
    char leading_space[256] = {0};
    int i = 0;
    for(;i < level; i++)
    {
        if (i == 0)
            strncat(leading_space, "|____", 5);
        else
            strncat(leading_space, "_____", 5);
    }
    DIR *dp = NULL;
    struct dirent *entry_ptr = NULL;

    dp = opendir(dir_name);
    if(dp == NULL)
    {
        return -1;
    }
    while(1)
    {
        entry_ptr = readdir(dp);
        if(entry_ptr == NULL)
        {
            break;
        }
        else
        {
            if(entry_ptr->d_type == DT_REG)
            {
                printf("%s%s\t(file)\n",leading_space, entry_ptr->d_name);
            }
            else if(entry_ptr->d_type == DT_DIR)
            {
                if((strcmp(entry_ptr->d_name, "..") != 0)
                  &&(strcmp(entry_ptr->d_name, ".") != 0)

                )
                {
                    printf("%s%s\t(dir)\n",leading_space, entry_ptr->d_name);
                    strncat(buffer, dir_name, strlen(dir_name));
                    strncat(buffer, "/", 1);
                    strncat(buffer, entry_ptr->d_name, strlen(entry_ptr->d_name));
                    res = print_dir_content(buffer, level + 1);
                    if(res != 0)
                    {
                        printf("failed to print dir: %s\n", entry_ptr->d_name);
                        closedir(dp) ;;
                        exit(-1);
                    }
                }
            }
            else
            {
                printf("%s\t(unknow)(%u)\n", entry_ptr->d_name, entry_ptr->d_type);
            }
        }
    }
    closedir(dp) ;
    return 0;
}

int main(int argc, char* argv[]) 
{
    long long i = 5;
    //long long num = 23;
    if(argc != 2)
    {
        printf("usage: %s dir_name\n", argv[0]);
        return -1;
    }
    print_dir_content(argv[1], 1);
    JUNX_UNUSED(argc);
    JUNX_UNUSED(argv);

    junx_vector* jvec = get_junx_vector_static()->create(5, sizeof(long long));
    if (NULL == jvec)
    {
        return -1;
    }


    //process_junx_objct((junx_object*)(jvec));

    for (; i < 100; i++)
    {
        JUNX_DER_METHODS(jvec, junx_vector_static)->push_front(jvec, &i);
    }
    //JUNX_METHODS(jvec)->_debug((junx_object*)jvec);
    //for (i = 0; i < 100; i++) 
    {
        JUNX_DER_METHODS(jvec, junx_vector_static)->pop_front(jvec);
        
    }
    
    //JUNX_METHODS(jvec)->_debug((junx_object*)jvec);

    get_junx_vector_static()->destroy(&jvec);
    return 0;
}