% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v1.17, Sep 23, 2010


\title[EG A visualization architecture for astronomical scales]%
      {A visualization architecture for astronomical scales}

% for anonymous conference submission please enter your SUBMISSION ID
% instead of the author's name (and leave the affiliation blank) !!
\author[E. Axelsson \& A. Bock \& J. Costa \& A. Ynnerman]
       {E. Axelsson$^{1}$,
        A. Bock$^{1}$,
        J. Costa$^{2}$
        and A. Ynnerman$^{1}$
%        S. Spencer$^2$\thanks{Chairman Siggraph Publications Board}
        \\
% For Computer Graphics Forum: Please use the abbreviation of your first name.
         $^1$Linköping University\\
         $^2$New York University
       }

% ------------------------------------------------------------------------

% if the Editors-in-Chief have given you the data, you may uncomment
% the following five lines and insert it here
%
% \volume{27}   % the volume in which the issue will be published;
% \issue{1}     % the issue number of the publication
% \pStartPage{1}      % set starting page


%-------------------------------------------------------------------------
\begin{document}

\maketitle

\begin{abstract}



\end{abstract}

%-------------------------------------------------------------------------
\section{Introduction}
Thanks to humanity's collective discoveries of the world around us,
we are now aware of structures in the universe that are both tiny and huge.
The observable universe has a diameter of approximately $10^{27}$ meters,
while the Planck length, which is the length where quantum mechanical effects
make it impossible to distinguish between two physical locations, is approximately 
$10^{-35}$ meters.\\

Comprehending the vast differences of scales in the known universe is needless 
to say a huge challenge for us humans. Computer graphics and visualization have
prooven to be immensively valuable tools for communicating scientific findings
and contextualizing informaiton. However, even in our virtual worlds, huge scale
differences are difficult to represent, due to limitations in numeric precision
in state-of-the-art hardware and software architectures.

\section{Limitations of the Traditional Rendering Pipeline}
- Floating point numbers \\
- Catastrophic cancellation \\
- depth buffer precision \\

\section{Related work}
Hanson et Al. introduces the concept of Power scaled coordinates, where ... \\

Prominent examples of applications where huge scale differences are visualized 
together include movie productions, such as "Powers of Ten" [citaition needed],
All We Are [citation needed], as well as interactive software for digital planetariums,
notable examples being Uniview from Sciss, DigiStar from Evans and Sutherland, [citations and more examples].

- Describe Scale Graph (find a good reference) \\
- Problems with stereo rendering in ScaleGraph \\
- Double and Quad precision rendering \\

\section{Dynamic Scene Graph}

\section{Depth Buffer}

\subsection{Volume rendering and Order Independent Transparency}

\section{Result}


%-------------------------------------------------------------------------
\subsection{Conclusions}

Please direct any questions to the production editor in charge of
these proceedings.

\section{Possible references}
Very Large Scale Visualization Methods for
Astrophysical Data. \\
Andrew J. Hanson, Chi-Wing Fu, and Eric A. Wernert \\
\url{http://www.cs.indiana.edu/~hansona/papers/vissym00.pdf}


Progress in Rendering and Modeling for Digital Planetariums \\
M. Magnor
and P. Sen
and J. Kniss
and E.Angel
and S. Wenger \\
\url{https://www.ece.ucsb.edu/~psen/Papers/EG10_DigitalPlanetariums.pdf}


%-------------------------------------------------------------------------

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{egbibsample}

%-------------------------------------------------------------------------

\end{document}
