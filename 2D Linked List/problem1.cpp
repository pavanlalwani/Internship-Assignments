#include<iostream>
using namespace std;
#define N 3     //For N by N (dimensions) of matrix. N equals 3. 

/*
Each node in 2D Linked List consists of 
two links (right and down) instead of just one.
Head pointer stores the first element of first row
and can be used to traverse the entire list.

Example representation of a 3X3 matrix with links:
matrix = {{1,2,3}, {4,5,6}, {7,8,9}}

HEAD-->1 --> 2 --> 3 --> NULL
       |     |     |
       v     v     v
       4 --> 5 --> 6 --> NULL
       |     |     |
       v     v     v
       7 --> 8 --> 9 --> NULL   
       |     |     |                  
       v     v     v
       NULL  NULL  NULL

The algorithms in our code are generalised for 
NXN matrix, they are not limited to 3X3 matrices.
Tested and verified by passing 2X2 and 4X4 matrices 
and setting N to the corresponding value.
 */

class LinkedList_2D{
    private:

    struct node{
        int data;
        node* right;
        node* down;
    };    
    node* head;

    node* getNode(){    //Allocate memory for new node
        node* temp=new node;
        temp->right=NULL;
        temp->down=NULL;
        return temp;
    }

    void swap(node *a,node *b){ 
        int temp;   //Swap data of two nodes
        temp=a->data;
        a->data=b->data;
        b->data=temp;
    }

    public:

    LinkedList_2D(){
        head = NULL; 
    }

    node* create2DLinkedList(int mat[N][N]){
        node* aboveTemp = NULL; //aboveTemp stores node just above temp
        node* rowTemp = NULL;   //rowTemp stores head pointer of that row
        node* temp = NULL;

        for(int i = 0; i < N; i++){
            if(rowTemp == NULL)
                rowTemp = getNode();//memory allocation for first row of the matrix
            temp = rowTemp;
            if(head == NULL)
                head = rowTemp;//Initializing head to point to first row
            for(int j = 0; j < N; j++){
                temp->data = mat[i][j];
                if(aboveTemp != NULL){
                    aboveTemp->down = temp;//node directly above "temp" now points to "temp"
                    aboveTemp = aboveTemp->right;
                }
                if(j < N-1)
                    temp->right = getNode();
                temp = temp->right;
            }
            aboveTemp = rowTemp;
            if(i < N-1)
                rowTemp->down = getNode();
            rowTemp = rowTemp->down;
        }
    }    

    void display(){
        node *rightTemp,*downTemp = head;   //Temporary pointers to traverse list
        cout<<"\n";
        while(downTemp != NULL){
            rightTemp = downTemp;
            while(rightTemp != NULL){
                cout<<rightTemp->data<<"\t";
                rightTemp=rightTemp->right;
            }
            cout<<"\n";
            downTemp=downTemp->down;
        }
        cout<<"\n";
    }
    
    int perimeter(){
        //Returns sum of all the perimeter elements
        int sum = 0;
        node *temp = head;
        //Traverse towards right till last element, then downwards
        while(temp->right !=NULL){
            sum += temp->data;
            temp = temp->right;
        }
        while(temp->down != NULL){
            sum += temp->data;
            temp = temp->down;
        }
        
        temp = head;    //Back to first row, first column
        //Traverse downwards till last element, then right
        while(temp->down != NULL){
            sum += temp->data;
            temp = temp->down;
        }
        while(temp->right != NULL){
            sum += temp->data;
            temp = temp->right;
        }
        sum += temp->data;  //Add last element of last row
        sum -= head->data;  //Since first row first element is counted twice
        return sum;
    }
    
    int diagonal(){
        int sumOfDiagonalElements = 0;
        node *temp = head;
        while(temp != NULL){
            sumOfDiagonalElements += temp->data;
            temp = temp->right;
            if(temp != NULL)    //Move to next element in diagonal, if any.
                temp=temp->down;
        }
        return sumOfDiagonalElements;
    }
    
    void transpose(){
        //Store transpose of matrix in the list
        node *temp,*downTemp,*rightTemp;
        temp = head;
        while(temp != NULL){
            rightTemp = temp->right;
            downTemp = temp->down;
            while(downTemp != NULL && rightTemp != NULL){
                swap(downTemp,rightTemp);
                rightTemp = rightTemp->right;
                downTemp = downTemp->down;
            }
            temp = temp->right;
            if(temp != NULL)
                temp=temp->down;
        }
    }

};

int main(){
    int mat[N][N] = {{1,2,3},{4,5,6},{7,8,9}};
    LinkedList_2D list;
    list.create2DLinkedList(mat);
    cout<<"2D Linked List created!\n";
    list.display();
    cout<<"Perimeter of matrix: "<<list.perimeter()<<endl;
    cout<<"Sum of diagonal elements: "<<list.diagonal()<<endl;
    list.transpose();
    cout<<"Transpose of matrix: \n";
    list.display();
    return 0;
}

