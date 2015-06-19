ds_project
==========
####&nbsp;&nbsp;&nbsp;&nbsp;estimate algorithm of relation between words
<br/>

About
-----
&nbsp;&nbsp;This project is designing and realizing the estimate algorithm of relation between words. (google's word2vec is similar to ours)<br/>
&nbsp;&nbsp;It is conducted with [minwookoo](https://github.com/minwookoo) as a sogang university data structure class term project.
<br/><br/>

A brief description of the algorithm
-----
1. Analyze the text file and select axis words properly.
2. Count the number of simultaneous appearance in a similar position between all the words and axis words in the text file.
3. Represent the result of (2) in the vector space. (axis words are basis of vector space)
<br/><br/>

Composition of project
-----
####&nbsp;&nbsp;1. analysis.c
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;analyze text file using our algorithm and export the result of analyzing.
####&nbsp;&nbsp;2. query.c
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;perform user's query using the result of analyzing.
<br/><br/>

Project Team - KK (Kwon & Koo)
-----
####[taeguk](https://github.com/taeguk)<br/>
####[minwookoo](https://github.com/minwookoo)
<br/>

Some images of the execution
-----
####Use [text8](http://mattmahoney.net/dc/textdata) as a text file
###1. analysis.c <br/>
  ![analysis-1][analysis-1]
  ![analysis-2][analysis-2]

###2. query.c
  * <h4>execute</h4>
    ![query-e][query-e]
    ![query-m][query-m]
  * <h4>menu 1</h4>
    ![query-1][query-1]
  * <h4>menu 2</h4>
    ![query-2][query-2]
  * <h4>menu 3</h4>
    ![query-3][query-3]
  * <h4>menu 4</h4>
    ![query-4-1][query-4-1]
    ![query-4-2][query-4-2]
  * <h4>menu 5</h4>
    ![query-5-1][query-5-1]
    ![query-5-2][query-5-2]
    ![query-5-3][query-5-3]
    ![query-5-4][query-5-4]
    ![query-5-5][query-5-5]
    ![query-5-6][query-5-6]
    ![query-5-7][query-5-7]
    ![query-5-8][query-5-8]

[analysis-1]: https://github.com/taeguk/ds_project/blob/master/img/analysis-1.png
[analysis-2]: https://github.com/taeguk/ds_project/blob/master/img/analysis-2.png
[query-e]: https://github.com/taeguk/ds_project/blob/master/img/query-e.png
[query-m]: https://github.com/taeguk/ds_project/blob/master/img/query-m.png
[query-1]: https://github.com/taeguk/ds_project/blob/master/img/query-1.png
[query-2]: https://github.com/taeguk/ds_project/blob/master/img/query-2.png
[query-3]: https://github.com/taeguk/ds_project/blob/master/img/query-3.png
[query-4-1]: https://github.com/taeguk/ds_project/blob/master/img/query-4-1.png
[query-4-2]: https://github.com/taeguk/ds_project/blob/master/img/query-4-2.png
[query-5-1]: https://github.com/taeguk/ds_project/blob/master/img/query-5-1.png
[query-5-2]: https://github.com/taeguk/ds_project/blob/master/img/query-5-2.png
[query-5-3]: https://github.com/taeguk/ds_project/blob/master/img/query-5-3.png
[query-5-4]: https://github.com/taeguk/ds_project/blob/master/img/query-5-4.png
[query-5-5]: https://github.com/taeguk/ds_project/blob/master/img/query-5-5.png
[query-5-6]: https://github.com/taeguk/ds_project/blob/master/img/query-5-6.png
[query-5-7]: https://github.com/taeguk/ds_project/blob/master/img/query-5-7.png
[query-5-8]: https://github.com/taeguk/ds_project/blob/master/img/query-5-8.png
