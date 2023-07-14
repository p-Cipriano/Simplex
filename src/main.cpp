#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

void row_multiply(vector<vector<double>> &matrix, int matrix_width, int row, double value){
    for(int j = 0; j<matrix_width; j++){
        matrix[row][j] = matrix[row][j]*value;
    }
}
void row_add(vector<vector<double>> &matrix, int matrix_width, int row_to_change, double lambda, int aux_row){
    for(int j = 0; j<matrix_width; j++){
        matrix[row_to_change][j] = matrix[row_to_change][j] + lambda*matrix[aux_row][j];
    }
}
void pivot(vector<vector<double>> &matrix, vector<vector<double>> &record, int matrix_width, int matrix_height, int pivot_x, int pivot_y){
    double aux = 1/matrix[pivot_x][pivot_y];
    row_multiply(matrix, matrix_width+1, pivot_x, aux);
    row_multiply(record, matrix_height, pivot_x, aux);
    for(int i = 0; i<matrix_height+1; i++){
        if(i!=pivot_x){
            aux = -matrix[i][pivot_y];
            row_add(matrix, matrix_width+1, i, aux, pivot_x);
            row_add(record, matrix_height, i, aux, pivot_x);
        }
    }
}
void print(vector<vector<double>> &matrix, int matrix_height, int matrix_width){
    for(int i = 0; i<matrix_height; i++){
        for(int j = 0; j<matrix_width; j++){
            cout << matrix[i][j];
            cout << " ";
        }
        cout<<endl;
    }
}
void print(vector<int> x, int size){
    for(int j = 0; j<size; j++)
        cout << x[j] << " ";
    cout<<endl;
}

int simplex(vector<vector<double>> &record, vector<vector<double>> &matrix,vector<int> &x, int n, int m){
    
    int pivot_column = -1;
    int pivot_row;
    
    while(1){
        pivot_row = -1;
        //Condição de parada e coluna para pivotear
        for(int j = 0; j < m; j++){
            if(matrix[0][j] < -pow(10,-6)){
                pivot_column = j;
                break;
            }
            else if(j==m-1)
                return -2;//ótima
        }
        
        //Acha a linha para pivotear
        for(int i = 1; i<n+1; i++){
            if(matrix[i][pivot_column]>pow(10,-6)){
                if(pivot_row<0 || matrix[i][m]*matrix[pivot_row][pivot_column]<matrix[i][pivot_column]*matrix[pivot_row][m])
                    pivot_row = i;
            }
            else if(i==n && pivot_row==-1)
                return pivot_column;//ilimitada
        }
        
        //Atualiza solução
        for(int j = 0; j<m+1 ; j++){
            if(x[j]==pivot_row){
                x[j]=-1;
                break;
            }
        }
        x[pivot_column] = pivot_row;
        
        //Deixa em forma canonica no novo pivot
        pivot(matrix, record, m, n, pivot_row, pivot_column);
        //print(matrix,n+1,m+1);
        //cout<<endl;

    }
}

int main(){
    
    int n,m,columns;
    int a;
    cin>>n>>m;
    columns = 2*n+m+1;
    
    vector<vector<double>> record(n+1, vector<double>(n));
    vector<vector<double>> matrix(n+1, vector<double>(columns));
    vector<double> c(n+m);
    
    for(int i = 0; i<n+1; i++){
        for(int j = 0; j<m+1; j++){
            if(i==0 && j==m)
                break;
            
            cin>>a;
            
            if(i==0){
                a *= -1;
                c[j] = a;
            }
            else if(j<m && i!=0)
                matrix[i][j] = a;
            else
                matrix[i][columns-1] = a;
        }
    }
    
    vector<int> x(2*n+m);
    for(int j = 0; j<m; j++)
        x[j] = -1;
    
    //Completando certificado de válidade e optimalidade
    for(int i=1;i<n+1;i++){
        matrix[i][i-1+m] = 1; //folga
        if(matrix[i][columns-1] >= 0){
            record[i][i-1] = 1;
            x[i-1+m] = i;
            x[i-1+m+n] = -1;
        }
        else{
            row_multiply(matrix, columns, i, -1);
            matrix[i][i-1+m+n] = 1;//nao_basicas
            record[i][i-1] = -1;
            x[i-1+m] = -1;
            x[i-1+m+n] = i;
        }
        matrix[0][m+n+i-1] = 1;
    }

    // Deixar em forma canonica
    //Zerar -cTb
    for(int i = 1; i<n+1; i++){
        if(x[i+m-1]==-1){
        row_add(matrix, 2*n+m+1, 0, -1, i);
        row_add(record, n, 0, -1, i);
        }
    }
    
    //print(matrix,n+1,columns);
    //print(record,n+1,n);
    //print(x,m+2*n);
    
    int cas = simplex(record,matrix,x,n,columns-1);
    
    //print(matrix,n+1,columns);
    //print(record,n+1,n);
    //print(x,m+2*n);
    
    bool LD = true;
    
    if(matrix[0][columns-1] < -pow(10,-6))
        cas = -1;
    
    else{
        //Remove variáveis artificiais
        for(int j = m+n; j<m+2*n; j++){
            if(x[j]!=-1){
                //B=0 mas linha é LI -> qualquer valor não zero serve
                for(int k = 0; k< m+n; k++){
                    if(matrix[x[j]][k]!=0){
                        x[k] = x[j];
                        x[j] = -1;
                        if(matrix[x[k]][k]<-pow(10,-6)){
                            row_multiply(matrix, columns, x[k], -1);
                            row_multiply(record, n, x[k], -1);
                        }
                        pivot(matrix,record,m+n,n,x[k],k);
                        LD = false;
                        break;
                    }
                }
                if(LD){
                    for(int i = 0; i<n+1; i++){
                        for(int k = 0; k<n; k++){
                            if(k!=x[j])
                                record[i][k] = record[i][k]+record[i][x[j]-1]*record[x[j]][k];
                        }
                        record[i][x[j]-1]=0;
                    }
                    row_multiply(matrix, 2*n+m, x[j], 0);
                    row_multiply(record, n, x[j], 0);
                    x[j] = -1;
                }
            }
            LD = true;
        }
        //Reintroduzir função objetivo
        for(int i = 0; i < n+1; i++)
            matrix[i][n+m] = matrix[i][columns-1];
        
        for(int j = 0; j < m+n; j++){
            matrix[0][j] = matrix[0][j]+c[j];
            if(x[j]!=-1){
                row_add(record, n, 0, -matrix[0][j], x[j]);
                row_add(matrix, n+m+1, 0, -matrix[0][j], x[j]);
            }
        }
        
        //print(matrix,n+1,n+m+1);
        //print(record,n+1,n);
        //print(x,m+n);
        
        cas = simplex(record,matrix,x,n,m+n);
        
        //print(matrix,n+1,n+m+1);
        //print(record,n+1,n);
        //print(x,m+n);
    }
    
    switch(cas){
        case -1:
            cout<<"inviavel"<<endl;
            
            print(record,1,n);
            break;

        case -2:
            cout<<"otima"<<endl;
            
            cout << matrix[0][m+n];
            cout<<endl;
            for(int j = 0; j<m; j++){
                if(x[j]==-1)
                    cout << 0;
                else
                    cout<<matrix[x[j]][m+n];
                cout << " ";
            }
            cout<<endl;
            print(record,1,n);
            break;
            
            
        default:
            cout<<"ilimitada"<<endl;
            
            for(int j = 0; j<m; j++){
                if(x[j]!=-1)
                    cout<<matrix[x[j]][n+m];
                else
                    cout<<0;
                cout << " ";
            }
            cout<<endl;
            for(int j=0;j<m;j++){
                if(j==cas)
                    cout<<1;
                else if(x[j]!=-1)
                    cout<<-matrix[x[j]][cas];
                else
                    cout<<0;
                cout<<" ";
            }
            cout<<endl;
            break;
    }
    return 0;
}
