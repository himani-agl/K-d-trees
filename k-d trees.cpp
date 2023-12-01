#include<iostream>
#include<cmath>
#include <math.h>
#include <stdlib.h>

using namespace std;

#define k 5
int no_of_neighbours = 0;

typedef struct node{
    float point[k];
    node* left;
    node* right;
}node;

node* newNode(float arr[]){
    node* temp = new node;
    for(int i=0;i<k;i++){
        temp->point[i] = arr[i];
    }
    temp->left = temp->right = NULL;
    return temp;
}

//function to calculate euclidean distance between two points
float eucDist(node* a, node* b){
    float dist = 0;
    for(int i=0;i<k;i++){
        dist += (a->point[i] - b->point[i]) * (a->point[i] - b->point[i]);
    }
    return sqrt(dist);
}

//function to evaluate manhattan disatance between two points
// float manDist(node* a, node* b){
//     float dist = 0;
//     for(int i=0;i<k;i++){
//         dist += abs(a->point[i] - b->point[i]);
//     }
//     return dist;
// }

//function to evaluate max norm disatance between two points

// float maxNormDist(node* a, node* b){
//     float dist = 0;
//     for(int i=0;i<k;i++){
//         if(abs(a->point[i] - b->point[i]) > dist){
//             dist = abs(a->point[i] - b->point[i]);
//         }
//     }
//     return dist;
// }


//function to print the point of given node
void printPoint(node* z) 
{
    cout << "(";
    for (int i = 0; i < k; i++) {
        cout << z->point[i];
        if (i != k - 1) {
            cout << ",";
        }
    }
    cout << ") ";
}

// function to insert node in kd tree
node* insert(node* root, float point[], unsigned depth){
    if(root == NULL){
        return newNode(point);
    }
    unsigned cd = depth % k;
    if(point[cd] < (root->point[cd])){
        root->left = insert(root->left, point, depth+1);
    }
    else{
        root->right = insert(root->right, point, depth+1);
    }
    return root;
}

//function to find the nearest neighbour of query q
node* nearestNeighbour(node* root, node* q, unsigned depth) {
    if (root == NULL) {
        return NULL;
    }

    unsigned cd = depth % k;
    node* goodSide = NULL;
    node* badSide = NULL;

    // Determine which side to traverse based on the comparison of the current dimension
    if (q->point[cd] < root->point[cd]) {
        goodSide = root->left;
        badSide = root->right;
    } else {
        goodSide = root->right;
        badSide = root->left;
    }

    // Recursively search the good side
    node* best = nearestNeighbour(goodSide, q, depth + 1);

    // Check if the distance to the current node is shorter than the best distance found so far
    if (best == NULL || eucDist(q, best) > eucDist(q, root)) {
        best = root;
    }

    // Check if the distance to the bad side is shorter than the best distance found so far
    if (badSide != NULL && eucDist(q, best) > abs(q->point[cd] - root->point[cd])) {
        node* temp = nearestNeighbour(badSide, q, depth + 1);
        if (temp != NULL && eucDist(q, best) > eucDist(q, temp)) {
            best = temp;
        }
    }

    return best;
}

// function to find the nearest neighbour of query q by brute force method
node* nearestNeighbourBruteForce(node* root, node* q){
    if(root == NULL){
        return NULL;
    }
    node* best = root;
    float bestDist = eucDist(q, root);
    float dist;
    if(root->left != NULL){
        node* temp = nearestNeighbourBruteForce(root->left, q);
        dist = eucDist(q, temp);
        if(dist < bestDist){
            best = temp;
            bestDist = dist;
        }
    }
    if(root->right != NULL){
        node* temp = nearestNeighbourBruteForce(root->right, q);
        dist = eucDist(q, temp);
        if(dist < bestDist){
            best = temp;
            bestDist = dist;
        }
    }
    return best;
}

// function to find and print the coordinates of the nearest neighbour of query q within the radius r
void nearestNeighbourRadius(node* root, node* q, unsigned depth, float r){
    if(root == NULL){
        return;
    }
    unsigned cd = depth % k;
    node* goodSide = NULL;
    node* badSide = NULL;
    if(q->point[cd] < root->point[cd]){
        goodSide = root->left;
        badSide = root->right;
    }
    else{
        goodSide = root->right;
        badSide = root->left;
    }
    nearestNeighbourRadius(goodSide, q, depth+1, r);
    if(eucDist(q, root) <= r){
        printPoint(root);
        no_of_neighbours++;
    }
    if(eucDist(q, root) <= r){
        nearestNeighbourRadius(badSide, q, depth+1, r);
    }
}

// function to print the kd tree (inorder)
void printTree(node* root){
    if(root == NULL){
        return;
    }
    printTree(root->left);
    printPoint(root);
    printTree(root->right);
}

int main(){
    node* root=NULL;
    int n=200;
    srand(time(0));
    //build a kd trre with 200 random points
    for(int i=0;i<n;i++){
        float arr[k];
        for(int j=0;j<k;j++){
            float r1 = rand() % 100;
		    int	r2 = rand() % 2;
            arr[j] = (r1 / 10.0) * pow(-1, r2);
        }
        root = insert(root, arr, 0);
    }

    cout<<"Inorder traversal of kd tree: "<<endl;
    printTree(root);
    cout<<endl;

    //query point
    float q[k];
    for(int i=0;i<k;i++){
         float r1 = rand() % 100;
		    int	r2 = rand() % 2;
            q[i] = (r1 / 10.0) * pow(-1, r2);
    }

    cout<<"Query point: ";
    for(int i=0;i<k;i++){
        cout<<q[i]<<" ";
    }
    cout<<endl;

    //nearest neighbour of query point
    node* nn = nearestNeighbour(root, newNode(q), 0);
    cout<<"Nearest neighbour of query point: ";
    printPoint(nn);
    cout<<endl;
     
    //nearest neighbour of query point by brute force method
    node* nnBruteForce = nearestNeighbourBruteForce(root, newNode(q));
    cout << "Nearest neighbour of query point (Brute Force): ";
    printPoint(nnBruteForce);
    cout << endl;

    //nearest neighbours of query point within radius 10
    cout<<"Nearest neighbours of query point within radius 10 : "<<endl;
    nearestNeighbourRadius(root, newNode(q),0,10);
    cout<<endl;
    cout<<"Number of neighbours within radius 10: "<<no_of_neighbours<<endl;

    delete(root);
 
 return 0;
}
