# Pupol

# Project Goal
The purpose of the project is to read the file in parallel and work with the threads

# Solution
<li>
The method is that a distance is considered for each string and the program gives the string a new area in which the strings are not involved. There is a value for the total read area of ​​the existing string.
</li>
<li>
Each time a new request is completed, the value of complet will increase, which means that it is working from 0 to this new interval.
</li>

# Example of Run code
<ul>
    <il>
        Task 0 Range[0, 8196] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </il>
    <il>
        Task 2 Range[16392, 24588] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </il>
    <li>
        Task 3 Range[24588, 32784] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 0 Range[0, 8196] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 2 Range[16392, 24588] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 3 Range[24588, 32784] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 3 Range[24588, 32784] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 3 Range[24588, 32784] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
    <il>
        Task 3 Range[24588, 32784] : "abc,pixar,xyz,Disneyland,abc,Ubuntu OS,512"
    </li>
</ul>

Tasks Complet.
Time : 0.357534 Seconds To Execute.
