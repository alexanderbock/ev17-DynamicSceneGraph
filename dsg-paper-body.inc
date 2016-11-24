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
and contextualizing information.

Notable examples of applications where huge scale differences are visualized 
together include movie productions, such as \emph{Powers of Ten}\cite{powersOfTen} from 1977,
All We Are [citation needed], as well as interactive software for digital planetariums,
such as Uniview from Sciss, DigiStar from Evans and Sutherland, [citations and more examples].

Even in our virtual worlds, huge scale
differences are difficult to represent, due to limitations in numerical precision
in state-of-the-art hardware and software architectures.

In movie productions and visualization software with limited interaction possibilities,
producers and developers may use various compositing techniques to blend
between large and small scenes, to account for the individual cases where a transition
between scales is required. 

Allowing the user to navigate around in a dataset with greater
freedom requires a more general system that can handle scale differences automatically.
There are several challenges with developing such a system, where the most prominent
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

\section{Related work}
The concept of \emph{Power Scaled Coordinates}, introduced by Hanson et al. \cite{hanson2000very},
addresses some of the challenges with multi-scale visualization. By using the fourth component of a homogenious coordinate vector to encode powers of ten, the proposed system can operate with floating point numbers close to unit scale. The method solves the issue of exponent overflow that occurs when performing operations on large and close-to-zero vectors. However, this is achieved by adding complexity in terms of an extra layer of logarithmic representation on top of regular floating point vectors, which already have the property of encoding scale logarithmically. 

The Power Scaled Coordinates method is based on floating point representation of numbers, which yield high precision close to zero, and decreasing precision with larger values. Hence, the method achieves high precision close to the origin of the scene, but decreasing precision for coordinates further away. This is a viable solution for scenes where the high precision data is centered in one region. However, the challenge of representing mulitple regions in the virtual universe with high precision is not addressed in this work.

Further work by Fu et al. \cite{fu2007transparently} elaborate more on the concept of Power Scales Coordinates and introduce a depth scaling scheme to cover a wider range of distances than what is possible with a fixed point depth buffer and a conventional near and far plane. The depth buffer range is divided into three regions, where small and huge depth values are remapped using logarithmic scales, while values in the mid-range are stored linearly. While this method may be beneficial for some specific type of scene content, the method does not provide a generic way to select appropriate threshold values, and the choice of three separate regions with different mapping does not necessarily provide better results than one single logarithmic mapping.

The \emph{ScaleGraph} method, introduced by Klashed et al. \cite{KHECY10} as a part of the digital planetarium software Uniview, employs a scene graph structure with sub-scenes that the camera can belong to. The content of the current sub-scene is rendered in its local coordinate system, which maintains the precision of vertex coordinates, and allows for multiple regions in the universe to be represented with high precision. When rendering content outside the current sub-scene, objects are translated onto the bounding sphere of the scene, and rescaled to compensate for perspective effects. Vectors between sub-scenes are computed by traversing the scenegraph through the closest common ancestor in the tree, to avoid introducing precision errors caused by handling unnecessarily large numbers. By moving objects onto the bounding sphere of the current scene, object depths are mapped to a managable range. However, the method does not provide a general solution for depth sorting objects that are outside the current sub-scene. Furthermore, the work does not address stereoscopic rendering of objects across sub-scenes.

\section{Background}
The co-evolution of graphics hardware, software and content, largely pushed forward by
the gaming industry has led to modern graphics processing units that are optimized for
operating efficiently on 32-bit floating point numbers, vectors and matrices.
In a standard computer graphics pipleline, $4x4$ matrices are used to represent coordinate
transformations operating on 4-dimensional homogenious coordinate vectors, with their components stored as floating point numbers. 

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing 32-bit floating point numbers \cite{zuras2008ieee} makes it possible to express numbers in a much larger range than integer types. While a 32-bit signed integer type can only represent numbers in the range 
$[-2^{31}, 2^{31} - 1]$, an IEEE754 float approximately support the range $[-2^{127}, 2^{127}]$.

In floating point numbers, values are represented as a sign, mantissa and exponent which gives the properties of high precision close to zero, decreasing precision for larger numbers, and a dramatic increase in value range. The IEEE754 single precision floating point format uses 1 sign bit $s$, 8 exponent bits and 23 mantissa bits $\mathbf{m}_i$. Equation \ref{eq:floatingpoint} yields the value $v$ represented by $s$, $\mathbf{m}$ and the integer $e \in [-127, 128]$ which is encoded in the exponent bits. 

\begin{equation} \label{eq:floatingpoint}
v = \begin{cases}
(-1)^s \cdot \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i} \cdot 2^{-126}, & \text{for } e = -127 \\
(-1)^s \cdot (1 + \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i}) \cdot 2^e, & \text{for } -126 \leq e \leq 127 \\
\text{Special numbers such as NaN and }\infty, & \text{for } e = 128
\end{cases}
\end{equation}

Only values $v$ that can be composed by these 32 bits are valid floating point numbers, and all other real values that result from computation need to be rounded before they are stored in this format. An upper bound of the absolute spacing between a floating point number $v$ and an adjacent one can be expressed as $\epsilon|v|$\cite{higham2002accuracy}, where the \emph{machine epsilon} $\epsilon$ is a constant given by $2^{-(n + 1)}$, and $n$ is the number of mantissa bits. Hence, the maximum error introduced by rounding of $v$ can be written as $u|v|$, where the rounding error coefficient $u = \frac{\epsilon}{2}$.
For IEEE754 float, $\epsilon = 2^{-24}\approx 5.96 \cdot 10^{-8}$, meaning that values are precise to about 7 significant figures in decimal notation.

\subsubsection{Increasing the bit depth}
An alternative data type for representing numbers is the IEEE754 64-bit floating-point format (double precision). The standard uses 1 sign bit, 11 exponent bits and 52 bits for the mantissa, which yields a machine epsilon of $\epsilon = 2^{-53}\approx 1.11 \cdot 10^{-16}$. In decimal notaiton, this corresponds to a precision of 15 significant figures. Using quadruple precision (128-bits), it is possible push the number of significant figures to 34. Increasing the bit depth will naturally require more computational resources.

\subsubsection{Interval arithmetic}
To reason about the precision of various floating point operations, it is possible to use interval arithmetic as a mathematical tool to track possible rounding errors. Due to the upper bound of the rounding error, a value $v$ may be rounded to a number $v' \in [v - u|v|, v + u|v|]$. Using interval arithmetic, the interval can be rewritten as $v(1 + u[-1,1])$.


The IEEE754 standard requires that the result of operations $+$, $-$, $\cdot$ and $/$ acting on any two floating point numbers should be the same result as an infinitely precise computation followed by a rounding to the closest representable float.  Throughout the following sections, $\oplus$, $\ominus$, $\odot$ and $\oslash$ will be used to denote floating point addition, subtraction, multiplication and division. In eqation \ref{eq:floatoperations}, we define the operations to yield the possible interval of real numbers that the corresponding floating point operation may evaluate to when applying floating point rounding.

\begin{equation} \label{eq:floatoperations}
\begin{split}
(a \oplus b) = (a + b)(1 + u[-1, 1]) \subseteq a + b + (a + b)u[-1, 1] \\
(a \ominus b) = (a - b)(1 + u[-1, 1]) \subseteq a - b + (a + b)u[-1, 1] \\
(a \odot b) = ab(1 + u[-1, 1]) \subseteq ab + abu[-1, 1] \\
(a \oslash b) = \frac{a}{b}(1 + u[-1, 1]) \subseteq \frac{a}{b} + \frac{a}{b} u[-1, 1]
\end{split}
\end{equation}

When a result from one floating point operation is used in further computations,
the rounding errors will propagate through the whole series of operations.
By tracking upper bounds of these error sizes, we can show which type of series of operations that may lead to significant loss of presicion.

\subsubsection{Error intervals}
We introduce the notation $\mathds{E}(\lambda)$ to represent an error interval based on a vector $\lambda$ of non-negative integers $\lambda_i$ as in equation \ref{eq:errorterm}.

\begin{equation} \label{eq:errorterm}
\mathds{E}(\lambda) = \sum_{i=0}^\infty( \lambda_i u^i [-1, 1])
\end{equation}

Based on interval arithmetic, we derive two properties of the error intervals.

\begin{equation} \label{eq:errortermrules}
\begin{split}
\mathds{E}(\alpha) + \mathds{E}(\beta) = \mathds{E}(\alpha + \beta) \\ 
\mathds{E}(\alpha) \mathds{E}(\beta) = \mathds{E}(\alpha * \beta) \\ 
\end{split}
\end{equation} 

We also observe some properties of $\oplus$, and $\odot$ as they act on the two intervals $a(1 + \mathds{E}(\alpha))$ and $b( 1 + \mathds{E}(\beta))$. $u[-1,1]$ can be rewritten as $\mathds{E}(\delta)$, with all $\delta_i = 0$ except $\delta_1 = 1$.

\begin{equation} \label{eq:errorterm3} 
\begin{split}
a(1 + \mathds{E}(\alpha)) \oplus b( 1 + \mathds{E}(\beta)) = \\
= (a(1 + \mathds{E}(\alpha)) + b( 1 + \mathds{E}(\beta)))(1 + \mathds{E}(\delta)) = \\
= (a + a\mathds{E}(\alpha) + b + b\mathds{E}(\beta))(1 + \mathds{E}(\delta)) = \\
= a + a\mathds{E}(\alpha + \delta + (\delta * \delta)) + b + b\mathds{E}(\beta + \delta + (\delta * \delta))
\end{split}
\end{equation}

\begin{equation} \label{eq:errorterm4} 
\begin{split}
a(1 + \mathds{E}(\alpha)) \odot b( 1 + \mathds{E}(\beta)) = \\
= ab(1 + \mathds{E}(\alpha))(1 + \mathds{E}(\beta))(1 + \mathds{E}(\delta)) = \\
ab + ab\mathds{E}(\alpha + \beta + \gamma + \alpha*\beta + \alpha*\gamma + \beta*\gamma + \alpha*\beta*\gamma)
\end{split}
\end{equation}





\begin{equation} \label{eq:catastrophic-example}
\begin{split}
(a \oplus b) \ominus c = \big((a + b)(1 + u[-1, 1]) + c\big)(1 + u[-1, 1]) \subseteq \\
(a + b + c + au[-1, 1] + bu[-1, 1])(1 + u[-1, 1]) \subseteq \\
a + b + c + (|2au| + |2bu| + |cu| + |au^2| + |bu^2|)[-1, 1] \subseteq \\
\subseteq a + b + c + \mathds{E}_u(a) + \mathds{E}_u(b) + \mathds{E}_u(c)
\end{split}
\end{equation}

From equation \ref{eq:catastrophic-example}, we observe that the result of this specific floating point computation will be contained by an interval expressed with terms depending on $u$ and the input terms $a$, $b$ and $c$. 


Using this definition, we can rewrite equation \ref{eq:catastrophic-example} to 

for some $n$ and $\lambda_i$.


The positive integers $n$ and $c_i$, $i \in [1, n]$ will grow with the number of operations that involves the floating point number $x$. However, for our purposes, the number of operations will be orders of magnitude smaller than the scale differences of possible values of x. With this in mind, a few conclusions can be made about the properties of $\mathds{E}$.




\begin{equation} \label{eq:errorterm4} 
\begin{split}
(a + E_u(a)) \ominus (b + \mathds{E}_u(b) = \\
\big(((a + \mathds{E}_u(a)) - (b + \mathds{E}_u(b))\big)(1 + u[-1, 1]) \subseteq \\
\subseteq a - b + \mathds{E}_u(|a| + |b|)
\end{split}
\end{equation}

\begin{equation} \label{eq:errorterm5} 
\begin{split}
(a + E_u(a)) \odot (b + \mathds{E}_u(b) = \\
\big((a + \mathds{E}_u(a))(b + \mathds{E}_u(b))\big)(1 + u[-1, 1]) \subseteq \\
\subseteq ab + \mathds{E}_u(ab)
\end{split}
\end{equation}

TODO: Division.


\subsubsection{Catastrophic cancellation}
When evaluating the expression $(a + b) - b$ using floating point artithmetic, the value of $(a + b)$ will be subject to rounding before $b$ is subtracted. Using interval arithmetic, equation \ref{eq:floataddsub} gives the possible values of the evaluated expression.

\begin{equation} \label{eq:floataddsub}
\begin{split}
(a \oplus b) \ominus b \in
\big((a + b)(1 + u[-1, 1]) - b\big)(1 + u[-1, 1]) = \\
= a + u\big(2a + b + u(a + b)\big)[-1, 1]
\end{split}
\end{equation}

The interval size of possible outputs contains a term with a factor of $b$, indicating that small values of $a$ and large values of $b$ will result in increasing relative rounding errors. The loss of precision caused by adding and subtracting the same large number from a smaller one is commonly referred to as \emph{catastrophic cancellation}. 
In any rendering pipeline where e.g. matrix multiplications or vector additions result in this type of operations, there is a risk for floating point precision problems.

A scalar multiplication followed by a division by the same number (the expression $(a \cdot b) / b$) will \emph{not} yield a similar loss of precision, as shown in equation \ref{eq:floatmult}, where the size of the output interval is independent from $b$.

\begin{equation} \label{eq:floatmult}
\begin{split}
(a \odot b) \oslash b \in 
\big((a \cdot b)(1 + u[-1, 1]) / b\big)(1 + u[-1, 1]) = \\
= a + u(2a + u a)[-1, 1]
\end{split}
\end{equation}

\subsubsection{Exponent underflow and overflow}
The exponent range $[-126, 127]$ in 32-bit IEEE754 floats gives a possible scale range of approximately 76 orders of magnitude (OM). Computations that yield exponents outside that range will cause exponent underflow or overflow, and result in loss of data. A straight-forward implementation of computing the length of a vector using the Pythagorean theorem would need to store squared length in a floating point value as an intermediate step, halfing the range of valid input exponents and limiting the input scale differences to 38 OM.

The scale difference betweeen the Planck length and the diameter of the observable universe is approximately $62$ OM. While 32-bit floats are able to represent this scale difference, a convensional approach to computing lengths and normalizing vectors would break down.

\subsection{Coordinate transformations}
In computer graphics software, scene content is commonly organized in a \emph{scene graph}, which allow that coordinate transformations are represented hierarchically. Each node in the graph may contain a transformation, representable as a 4x4 matrix, that affects all its children. In order to transform an object from its local \emph{model coordinate system} to a global \emph{world coordinate system}, the transformation matrices of the ancestor nodes are concatenated to a \emph{model matrix} using matrix multiplication.

A \emph{view matrix} is also commonly generated based on a camera position and rotation expressed in the world coordinate system, and then used to transform world coordinates to \emph{view coordinates}, with the camera in the origin, and the z-axis parallel to the view direction.

To take perspective effects into account, coordinates are transformed into \emph{perspective space} using a \emph{projection matrix}. The transformation sets the fourth vector component to the depth of the coordinate. 
Only content that satisfies the equation $-w \le x, y, z \le w$ is rendered. The vector is divided by $w$ to compute the \emph{normalized device coordinates (NDC)}. This last step results in that objects farther away from the camera appear smaller on the screen. The x and y in NDC determines the object's position on the screen, with $(-1, -1)$ representing the bottom left corner and $(1, 1)$ upper right one.


\subsubsection{NDC precision}
Using column vectors, the NDC's $\mathbf{n}$ are derived by the model coordinates $\mathbf{x}$ in equation \ref{eq:mvp}.

\begin{align} \label{eq:mvp}
\mathbf{p} = PVM\mathbf{x} \\
\mathbf{n} = \frac{\mathbf{p}}{p_w}
\end{align}

Here, V is the view matrix, $M$ is the model matrix, and $\mathbf{x}$, $\mathbf{p}$ and $\mathbf{n}$ are homogenious coordinate vectors on the form $(x, y, z, w)^t$. The precision in normalized device coordinates depends on the collective effects of $M$, $V$, $P$ and the perspective division on $\mathbf{x}$. Using knowledge about floating point numbers as well as the properties of the matrices, it is possible to determine potential rounding errors in $\mathbf{n}$.

$M$ and $V$ are commonly composed from three types of transformations that are encoded in the scene graph: scaling, translation and rotation. A translation matrix $T$, a scaling matrix $S$ and a rotation matrix $R$ can be written as in equations \ref{eq:str-forms}, where $I$ is the identity matrix and $\mathbf{0}$ is the null vector.

\begin{equation} \label{eq:str-forms}
S = \mleft(
\begin{array}{c|c}
  S' & \mathbf{0}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright),
T = \mleft(
\begin{array}{c|c}
  I & \mathbf{t}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright),
R = \mleft(
\begin{array}{c|c}
  R' & \mathbf{0}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright)
\end{equation}

$S'$ is a diagonal $3x3$-matrix with the scaling components for $x$, $y$ and $z$ respectively.
While floating point matrix multiplacation implementations may differ in terms of order of operations, a possible version is the one described in equation \ref{eq:scaling}.


\begin{equation} \label{eq:scaling}
\mathbf{y}_c = S'_c \odot \mathbf{x}_c
\end{equation}


$https://www.opengl.org/registry/doc/GLSLangSpec.4.20.11.clean.pdf : page 92$


\subsection{Interval arithmetic}
To reason about the precision of various floating point operations, it is possible to use interval arithmetic as a mathematical tool to track possible rounding errors. Due to the upper bound of the rounding error, a value $v$ may be rounded to a number $v' \in [v - u|v|, v + u|v|]$. Using interval arithmetic, the interval can be rewritten as $v(1 + u[-1,1])$.






\subsection{View Frustums}


\subsubsection{Depth sorting}




Representing the known universe in a traditional scene graph structure with floating point matrices has obvious problems.


TODO: mention double and quad precision.

\subsection{Depth Sorting}


\section{Method}
\subsection{Dynamic Scene Graph}


\subsubsection{Dynamic camera attachment}

\subsubsection{Dynamic object attachment}

\subsubsection{Model view matrix computation}

\subsection{Depth Sorting}

\subsection{Volume rendering and Order Independent Transparency}

\section{Result}


%-------------------------------------------------------------------------
\section{Conclusions}


\section{Acknowledgements}



%-------------------------------------------------------------------------

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{dsg-bib}

%-------------------------------------------------------------------------

\end{document}
