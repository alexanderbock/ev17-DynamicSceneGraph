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
to say a huge challenge for us humans. Computer graphics and interactive visualization have
proven to be immensively valuable tools for communicating scientific findings
and contextualizing informaiton. However, even in our virtual worlds, huge scale
differences are difficult to represent, due to limitations in numerical precision
in state-of-the-art hardware and software architectures.

In movie productions and visualziation software with limited interaction possibilies,
producers and developers may use various compositing techniques to blend
between large and small scenes, to account for the individual cases where a transition
between scales is required. 

Allowing the user to navigate around in a dataset with greater
freedom requires a more general system that can handle scale differences automatically.
There are several challenges with developing such a system, where the the most prominent
one may be providing adequate numerical precision for all the components in the visualization pipeline.
Another important aspect is adapting the eye-separation used for stereoscopic rendering to enhance depth perception and improve immersion. From the perspective of software development and content creation,
it is desirable for this sort of system to facilitate creation of visualization content rather than introducing additional
complexity.

This work contributes to visualization of datasets with large scale differences by providing:
\begin{itemize}
\item A \emph{Precision-Adaptive Scene Graph} approach for representing objects and cameras to avoid presicion-related artifacts.
\item A scheme for adaptive eye-separation for stereoscopic rendering.
\item Minimal modifications on a conventional rendering pipeline.
\item Interoperability with volumetric rendering and order-independent-transparency.
\end{itemize}

\section{The Conventional Rendering Pipeline}
The co-evolution of graphics hardware, software and content, largely pushed forward by
the gaming industry has led to modern graphics processing units that are optimized for
operating efficiently on floating point numbers, vectors and matrices.
In a standard computer graphics pipleline, $4x4$ matrices are used to represent coordinate
transformations operating on 4-dimensional homogenious coordinate vectors, with their components stored as floating point numbers.

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing 32-bit floating point numbers makes it possible to express numbers in a much larger range than integer types. While a 32-bit signed integer type can only represent numbers in the range 
$[-2^{31}, 2^{31} - 1]$, an IEEE754 float approximately support the range $[-2^{127}, 2^{127}]$.

By representing values as a sign, mantissa and exponent is is possible to achieve high precision close to zero, decreasing precision for larger numbers and a dramatic increase in value range. The IEEE754 single precision floating point format uses 1 sign bit $s$, 8 exponent bits $e_i$ and 23 mantissa bits $m_i$. Equation \ref{floatingpoint} yields the value $v$ represented by $s$, $b$ and $e$, where $e \in [-127, 128]$ is encoded in the exponent bits $e_i$. 

\begin{equation} \label{floatingpoint}
v = \begin{cases}
(-1)^s \cdot \sum_{i = 1}^{23}b_i 2^{-i} \cdot 2^{-126}, & \text{for } e = -127 \\
(-1)^s \cdot (1 + \sum_{i = 1}^{23}b_i 2^{-i}) \cdot 2^e, & \text{for } -126 \leq e \leq 127 \\
\text{Special numbers such as NaN and }\infty, & \text{for } e = 128
\end{cases}
\end{equation}

Only values $v$ that can be composed by these 32 bits are valid floating point numbers, and all other real values that result from computation need to be rounded before they are stored in this format. The IEEE754 standard requires that the result of operations $+$, $-$, $*$ and $\div$ acting on any two floating point numbers should be the same result as when performing the operation with infinite precision and then rounding to the closest representable float\cite{???}. An upper bound of the absolute rounding error can be expressed as $\epsilon \cdot v$. The \emph{machine epsilon}, $\epsilon$ is a constant given by $2^{-(n + 1)}$, where $n$ is the number of mantissa bits.
For IEEE754 float, $\epsilon = 2^{-24}\approx 5.96 \cdot 10^{-8}$.

\[Accuracy and Stability of Numerical Algorithms\]


\subsubsection{Catastrophic cancellation}
The roudning errors that occur after arithmetic operations gives floating point numbers the property that for a sufficiently large number $a$ and a number $b$ sufficiently close to zero, $a + b$ will return the same floating point representation as $a$ alone. While the expected that $(a + b) - a$ should yield $b$, the expression will evaluate to $0$ using floating point artithmetic. This loss of presicion is commonly referred to as \emph{catastrophic cancellation}.

\subsubsection{Exponent under- and overflow}
Operations that cause overflow and underflow in the exponent of IEEE754 floats, result in loss of data. Given that the value range 

\subsection{Scene Graphs}
In software, matrices are commonly generated from a scene graph structure, allowing developers
to represent coordinate transformations hierarchically. Each node in the graph may contain a transformation that is 
also affecting all its children. To transform an object from its
local coordinate system to a global \emph{world coordinate system}, the transformation matrices
of the ancestor nodes are concatonated to a \emph{model matrix} using matrix multiplication. 

An additional \emph{view matrix} is also commonly generated based on a camera position and rotation expressed in the world coordinate system, and then used to transtorm world coordinates to \emph{view coordinates}.

Representing the known universe in a traditional scene graph structure with floating point matrices has obvious problems.

\subsection{Depth buffer}

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


Higham, Nicholas (2002). Accuracy and Stability of Numerical Algorithms (2 ed). SIAM. p. 37.
\url{http://ftp.demec.ufpr.br/CFD/bibliografia/Higham_2002_Accuracy%20and%20Stability%20of%20Numerical%20Algorithms.pdf}


%-------------------------------------------------------------------------

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{egbibsample}

%-------------------------------------------------------------------------

\end{document}