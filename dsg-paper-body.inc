% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v1.17, Sep 23, 2010


\title[EG A software architecture for interactive visualization of datasets with astronomical scale differences]%
      {A software architecture for interactive visualization of datasets with astronomical scale differences}

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
and contextualizing informaiton.

Notable examples of applications where huge scale differences are visualized 
together include movie productions, such as \emph{Powers of Ten}\cite{powersOfTen} from 1977,
All We Are [citation needed], as well as interactive software for digital planetariums,
such as Uniview from Sciss, DigiStar from Evans and Sutherland, [citations and more examples].

Even in our virtual worlds, huge scale
differences are difficult to represent, due to limitations in numerical precision
in state-of-the-art hardware and software architectures.

In movie productions and visualization software with limited interaction possibilies,
producers and developers may use various compositing techniques to blend
between large and small scenes, to account for the individual cases where a transition
between scales is required. 

Allowing the user to navigate around in a dataset with greater
freedom requires a more general system that can handle scale differences automatically.
There are several challenges with developing such a system, where the the most prominent
one may be providing adequate numerical precision for all the components in the visualization pipeline.
Another important aspect is adapting the eye-separation used for stereoscopic rendering to enhance depth perception and improve immersion. From the perspective of software development and content creation,
it is desirable for this sort of system to facilitate creation of visualization content rather than introducing additional complexity.

This work contributes to visualization of datasets with large scale differences by providing:
\begin{itemize}
\item A \emph{Precision-Adaptive Scene Graph} approach for representing objects and cameras to avoid presicion-related artifacts.
\item A scheme for adaptive eye-separation for stereoscopic rendering.
\item Minimal modifications on a conventional rendering pipeline.
\item Interoperability with volumetric rendering and order-independent-transparency.
\end{itemize}

\section{Related Work}
The concept of \emph{Power Scaled Coordinates}, introduced by Hanson et al. \cite{hanson2000very},
addresses some of the challenges with multi-scale visualization. By using the fourth component of a homogenious coordinate vector to encode powers of ten, the proposed system can operate with floating point numbers close to unit scale. The method does not enhance the precision of coordinate transformations carried out in the visualization pipeline, but solves the issue of exponent overflow that occurs when performing operations on large and close-to-zero vectors. However, this is achieved by adding complexity in terms of an extra layer of logarithmic representation on top of regular floating point vectors, which already have the property of encoding scale logarithmically. 

Further work by Fu et al. \cite{fu2007transparently} elaborate more on the concept of power scale coordinates and introduce a depth scaling scheme to cover a wider range of distances than what is possible with a fixed point depth buffer and a conventional near and far plane. The depth buffer range is divided into three regions, where small and huge depth values are remapped using logarithmic scales, while values in the mid-range are stored linearly. While this method may be beneficial for some specific type of scene content, the method does not provide a generic way to select appropriate threshold values, and the choice of three separate regions with different mapping does not necessarily provide better results than one single logarithmic mapping. 

The \emph{ScaleGraph} method, introduced by Klashed et al. \cite{KHECY10} as a part of the digital planetarium software Uniview, employs a scene graph structure with sub-scenes that the camera can belong to. The content of the current sub-scene is rendered in its local coordinate system, which maintains the precision of vertex coordinates. When rendering content outside the current sub-scene, objects are translated onto the bounding sphere of the scene, and rescaled to compensate for perspective effects. Vectors between sub-scenes are computed by traversing the scenegraph through the closest common ancestor in the tree, to avoid introducing precision errors caused by handling unnecessarily large numbers. By moving objects onto the bounding sphere of the current scene, object depths are mapped to a managable range. However, the method does not deal with depth sorting of objects that are outside the current sub-scene. Furthermore, the work does not address stereoscopic rendering of objects across sub-scenes.

\section{Background}
The co-evolution of graphics hardware, software and content, largely pushed forward by
the gaming industry has led to modern graphics processing units that are optimized for
operating efficiently on 32-bit floating point numbers, vectors and matrices.
In a standard computer graphics pipleline, $4x4$ matrices are used to represent coordinate
transformations operating on 4-dimensional homogenious coordinate vectors, with their components stored as floating point numbers. 

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing 32-bit floating point numbers \cite{zuras2008ieee} makes it possible to express numbers in a much larger range than integer types. While a 32-bit signed integer type can only represent numbers in the range 
$[-2^{31}, 2^{31} - 1]$, an IEEE754 float approximately support the range $[-2^{127}, 2^{127}]$.

In floating point numbers, values are represented as a sign, mantissa and exponent which gives the properties of high precision close to zero, decreasing precision for larger numbers, and a dramatic increase in value range. The IEEE754 single precision floating point format uses 1 sign bit $s$, 8 exponent bits and 23 mantissa bits $\mathbf{m}_i$. Equation \ref{floatingpoint} yields the value $v$ represented by $s$, $\mathbf{m}$ and the integer $e \in [-127, 128]$ which is encoded in the exponent bits. 

\begin{equation} \label{floatingpoint}
v = \begin{cases}
(-1)^s \cdot \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i} \cdot 2^{-126}, & \text{for } e = -127 \\
(-1)^s \cdot (1 + \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i}) \cdot 2^e, & \text{for } -126 \leq e \leq 127 \\
\text{Special numbers such as NaN and }\infty, & \text{for } e = 128
\end{cases}
\end{equation}

Only values $v$ that can be composed by these 32 bits are valid floating point numbers, and all other real values that result from computation need to be rounded before they are stored in this format. The IEEE754 standard requires that the result of operations $+$, $-$, $\cdot$ and $/$ acting on any two floating point numbers should be the same result as an infinitely precise computation followed by a rounding to the closest representable float. Throughout the following sections $\oplus$, $\ominus$, $\odot$ and $\oslash$ will be used to denote addition, subtraction, multiplication and division followed by rounding.

An upper bound of the absolute spacing between a floating point number $v$ and an adjacent one can be expressed as $\epsilon|v|$\cite{higham2002accuracy}, where the \emph{machine epsilon} $\epsilon$ is a constant given by $2^{-(n + 1)}$, and $n$ is the number of mantissa bits. Hence, the maximum error introduced by rounding of $v$ can be written as $u|v|$, where $u = \frac{\epsilon}{2}$.
For IEEE754 float, $\epsilon = 2^{-24}\approx 5.96 \cdot 10^{-8}$, meaning that values are precise to about seven significant figures in decimal notation.

\subsubsection{Interval arithmetic}
To reason about the precision of various floating point operations, it is possible to use interval arithmetic as a mathematical tool to track possible rounding errors. Due to the upper bound of the rounding error, a value $v$ may be rounded to a number $v' \in [v - u|v|, v + u|v|]$. Using interval arithmetic, the interval can be rewritten as $v(1 + u[-1,1])$.





\subsubsection{Catastrophic cancellation}
When evaluating the expression $(a + b) - b$ using floating point artithmetic, the value of $(a + b)$ will be subject to rounding before $b$ is subtracted. Using interval arithmetic, equation \ref{floatadd} gives the possible values of the evaluated expression.

\begin{equation} \label{floatadd}
\begin{split}
(a \oplus b) \ominus b \in
\big((a + b)(1 + u[-1, 1]) - b\big)(1 + u[-1, 1]) = \\
= a + u\big(2a + b + u(a + b)\big)[-1, 1]
\end{split}
\end{equation}

The interval size of possible outputs contains a term with a factor of $b$, indicating that small values of $a$ and large values of $b$ will result in increasing relative rounding errors. The loss of precision caused by adding and subtracting the same large number from a smaller one is commonly referred to as \emph{catastrophic cancellation}. 
In any rendering pipeline where e.g. matrix multiplications or vector additions result in this type of operations, there is a risk for floating point precision problems.

A scalar multiplication followed by a division by the same number (the expression $(a \cdot b) / b$) will \emph{not} yield a similar loss of precision, as shown in equation \ref{floatmult}, where the size of the output interval is independent from $b$.

\begin{equation} \label{floatmult}
\begin{split}
(a \odot b) \oslash b \in 
\big((a \cdot b)(1 + u[-1, 1]) / b\big)(1 + u[-1, 1]) = \\
= a + u(2a + u a)[-1, 1]
\end{split}
\end{equation}

\subsubsection{Exponent under- and overflow}
The exponent range $[-126, 127]$ in 32-bit IEEE754 floats gives a possible scale range of approximately 76 orders of magnitude (OM). Computations that yield exponents outside that range will cause exponent underflow or overflow, and result in loss of data. A straight-forward implementation of computing the length of a vector using the Pythagorean theorem would need to store squared length in a floating point value as an intermediate step, halfing the range of valid input exponents and limiting the input scale differences to 38 OM.

The scale difference betweeen the Planck length and the diameter of the observable universe is approximately $62$ OM. While 32-bit floats are able to represent this scale difference, a convensional approach to computing lengths and normalizing vectors would break down.

\subsection{Scene Graphs}
In software, matrices are commonly generated from a scene graph structure, allowing developers
to represent coordinate transformations hierarchically. Each node in the graph may contain a transformation that is 
also affecting all its children. In order to transform an object from its
local coordinate system to a global \emph{world coordinate system}, the transformation matrices
of the ancestor nodes are concatonated to a \emph{model matrix} using matrix multiplication. 

An additional \emph{view matrix} is also commonly generated based on a camera position and rotation expressed in the world coordinate system, and then used to transtorm world coordinates to \emph{view coordinates}.

Representing the known universe in a traditional scene graph structure with floating point matrices has obvious problems.


TODO: mention double and quad precision.

\subsection{Depth Sorting}


\section{Method}
\subsection{Dynamic Scene Graph}

\subsection{Depth Sorting}

\subsection{Volume rendering and Order Independent Transparency}

\section{Result}


%-------------------------------------------------------------------------
\subsection{Conclusions}

Please direct any questions to the production editor in charge of
these proceedings.

\section{Possible references}




%-------------------------------------------------------------------------

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{dsg-bib}

%-------------------------------------------------------------------------

\end{document}
