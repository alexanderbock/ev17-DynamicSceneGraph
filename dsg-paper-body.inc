% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v1.17, Sep 23, 2010


\title[EG A software architecture for interactive visualization of datasets of astronomical scale differences]%
      {A software architecture for interactive visualization of datasets of astronomical scale differences}

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
$10^{-35}$ meters.

Comprehending the vast differences of scales in the known universe is needless 
to say a huge challenge for us humans. Computer graphics and visualization have
prooven to be immensively valuable tools for communicating scientific findings
and contextualizing informaiton. However, even in our virtual worlds, huge scale
differences are difficult to represent, due to limitations in numeric precision
in state-of-the-art hardware and software architectures.

In movie productions and visualziation software with limited interaction possibilies,
producers and developers may use various compositing techniques to blend
between large and small scenes, to account for the individual cases where a transition
between scales is required. Allowing the user to navigate around in a dataset with more
freedom requires a more general system that can handle scale differences automatically.

\section{The Traditional Rendering Pipeline}
The co-evolution of graphics hardware, software and content, largely pushed forward by
the gaming industry has led to modern graphics processing units that are optimized for
operating efficiently on floating point numbers, vectors and matrices.
In a standard computer graphics pipleline, $4x4$ matrices are used to represent coordinate
transformations operating on 4-dimensional homogenious coordinate vectors.

\subsection{Scene Graphs}
In software, matrices are commonly generated from a scene graph structure, allowing developers
to represent transformations hierarchically. Each node in the graph may contain a
coordinate transform, also affecting all its children. To transform an object from its
local coordinate system to a global \emph{world coordinate system}, the transformation matrices
of the ancestor nodes are concatonated to a \emph{model matrix} using matrix multiplication. 

An additional \emph{view matrix} is also commonly generated based on a camera position and rotation expressed in the world coordinate system, and then used to transtorm world coordinates to \emph{view coordinates}.

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing floating point numbers deals with scale
differences by representing values as a sign, mantissa and exponent. The IEEE754
single precision floating point format uses 1 sign bit $s$, 8 exponent bits $e_i$ and 23 mantissa bits $m_i$.
The value $v$ represented by these bits can be epressed as in equation \ref{floatingpoint}, with a few exceptions including zero, infinity and denormal numbers (TODO.)

\begin{equation} \label{floatingpoint}
v = (-1)^s \cdot (1 + \sum_{i = 1}^{23}b_{23 - i} 2^{-i}) \cdot 2^{e-127}
\end{equation}




\subsection{Floating Point Numbers}


- Floating point numbers \\
- Catastrophic cancellation \\
- depth buffer precision \\

\section{Related Work}
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
