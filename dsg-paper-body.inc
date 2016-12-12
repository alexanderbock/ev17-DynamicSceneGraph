% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v1.17, Sep 23, 2010


%\title[EG Astronomical Scale Differences and Visualization]%
%      {Astronomical Scale Differences in Visualization}
% How about this? ---abock

% suggestion from Anders
\title{Dynamic Scene Graph:\\Enabling Scaling, Positioning, and Navigation in the Universe}
%\title[]{Dynamic Scene Graph: Avoiding Floating Point Inaccuracies in Large Scale Astrovisualization}

% for anonymous conference submission please enter your SUBMISSION ID
% instead of the author's name (and leave the affiliation blank) !!
\author[Axelsson \emph{et al.}]{Emil Axelsson$^{1}$, Jonathas Costa$^{2}$, Cl\'audio Silva$^{2}$, Carter Emmart$^{3}$, Alexander Bock$^{1}$, and Anders Ynnerman$^{1}$
        \\
% For Computer Graphics Forum: Please use the abbreviation of your first name.
         $^1$~Linköping University\\
         $^2$~New York University\\
         $^3$~American Museum of Natural History
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

\teaser{
  \centering
  \fbox{\includegraphics[width=\linewidth]{figure/teaser2.png}}
  \caption{Accurate rendering of a real scale model of the New Horizons spacecraft taking measurements on Pluto. The model of about $2$\,m size is shown in its correct location relative to Pluto, which is about $6\cdot 10^{12}$\,m from the coordinate system origin with the stars of the constellation Ophiucus (about $10^{18}$\,m) in their correct 3D positions. High precision is required for computing the correct location of images on the surface of Pluto as well as correctly rendering the shadow cylinders of both Pluto and its moon, Charon.}
\label{fig:teaser}
}

\maketitle

\begin{abstract}
In this work, we address the challenge of seamlessly visualizing astronomical data exhibiting huge scale differences in distance, size and resolution. One of the hurdles to overcome is accurate, fast and dynamic positioning and navigation to enable seamless scaling over orders magnitude, far beyond the precision of the floating point arithmetic on GPUs. To this end we propose a framework that utilizes a dynamically assigned frames of reference to provide the highest possible numerical precision for all salient objects in a scene graph. This makes it possible to smoothly navigate and interactively render, for example, surface structures on Mars and the Milky Way simultaneously. Our work is based on an analysis of tracking and quantification of the propagation precision errors through the computer graphics pipeline using interval arithmetic. Furthermore, we identify sources of precision degradation, leading to incorrect object positions in screen-space and z-fighting. Our proposed method operates without near and far planes while maintaining high depth precision through the use of floating point depth buffers. By providing interoperability with order-independent transparency algorithms, direct volume rendering, and stereoscopy, our approach is well suited for scientific visualization. We provide the mathematical background, a thorough description of the method, and a reference implementation.

%Old Abstract: In this work, we address the challenges of limited numerical range and precision of floating point numbers when simultaneously visualizing astronomical objects with huge scale differences, both in size and distance. Due to the limited precision of floating point numbers, it is impossible to represent comparatively small objects far away from the coordinate system origin. We propose a framework that utilizes a dynamically assigned coordinate system origin to provide the highest possible numerical precision for all salient objects in a scene graph. This makes it possible to navigate and interactively render, for example, surface structures on Mars and the Milky Way simultaneously. We track and quantify the propagation of numerical precision errors through the computer graphics pipeline using interval arithmetic for cases of large differences in scale. Furthermore, we identify sources of precision degradation, leading to incorrect object positions in screen-space and z-fighting, mathematically. Our proposed method operates without near and far planes while maintaining high depth precision through the use of floating point depth buffers. By providing interoperability with order-independent transparency algorithms, direct volume rendering, and stereoscopy, our approach is well suited for scientific visualization. We provide the mathematical background, a thorough description of the method, and a reference implementation.}

\end{abstract}

%-------------------------------------------------------------------------
\section{Introduction}
Over the past centuries the ability to observe and collect data representing the physical world has been one of the great accomplishments of mankind.
This includes observations ranging from the diminutive to the unimaginably large.
Everything we know and everything we can possibly know is constrained in size and distance by two boundaries.
The upper limit is given by the size of the observable universe with a comoving diameter of about $10^{27}$ meters.
The lower limit is the Planck length, at which the structure of space-time is dominated by quantum effects, of $10^{-35}$ meters.
Seamless positioning and navigation across this tremendous range of scales, roughly 60 orders of magnitude, generates many challenges to visualization systems.
Besides the issue of creating visual metaphors for objects and their interrelated positions, based on different abstraction levels and contracted distance representations, one challenge not immediately obvious is the limited precision of floating point numbers.
Following a na\"ive approach using a conventional computer graphics pipeline with a global coordinate origin at Earth's center, object locations cannot be described with the necessary precision, such as when viewing spacecraft at the edge of our solar system, or watching micrometer-scaled scans on the surface of Mars.
The location of objects thus need to be expressed in huge value ranges with great precision.
Current computer hardware is, however, limited to hardware accelerated computations of single and double precision floating point numbers, which are insufficient to represent microscopic objects and cosmological distances simultaneously.
This lack of precision causes well-known visual rendering artifacts, such as displaced vertices or z-fighting, and generate intricate problems when displaying volumetric content.
Furthermore, overflow and underflow of the available value ranges results in undesirable clipping of near and far geometry.

%Through various means of scientific discovery, humanity now has collected detailed information and measurements of structures in the surrounding cosmos that range from the diminutive to the unimaginably large. Everything we know and everything we can possibly know is constrained in size and distance by two boundaries. The upper boundary is given by the size of the observable universe with a comoving diameter of about $10^{27}$ meters. The lower boundary is the Planck length, at which the structure of space-time is dominated by quantum effects and the concept of locality loses physical meaning, with an approximate size of $10^{-35}$ meters. These two boundaries pose absolute limits to the resolution that a software system dealing data of any size has to support.

%Thanks to humanity's collective discoveries of the surrounding world, we are now aware of structures in the universe that are both tiny and huge. The observable universe has a diameter of approximately $10^{27}$ meters, while the Planck length, which is the length where quantum mechanical effects make it impossible to distinguish between two physical locations \joncomment{Maybe: ``...which is the length were the Heisenberg's uncertainty principle is present/observable, is...''}, is approximately $10^{-35}$ meters.



% Removed this part as it would be a large repeat of the related works section
%Previous work by the visualization community has provided solutions to some of these issues, such as the Power Scaled Coordinates by Hanson~\etal~\cite{hanson2000very} and the ScaleGraph by Klashed~\etal~\cite{KHECY10}. However, only partial solutions to the precision issues have been provided thus far and the root cause of these problems remains unsolved. \anderscomment{Hansen would disagree and kill the paper here if he were the reviewer, which is likely. In what way are they partial? We have to very precise here to make a case}

In this work, we propose a general framework, a dynamic scene graph, which enables fast and accurate scaling, positioning, and navigation without significant loss of precision.
The concept is applicable to a variety of application domains with large scale differences and not limited to astronomical data.
The framework is based on an analysis of the sources of precision errors and range overflow problems in a conventional computer graphics pipeline.
Using interval arithmetic to track floating point rounding errors, we are able to identify which set of operations that may introduce precision problems.
We combine this information with the observation that high level of detail is only required for nearby objects.
The underlying technique relies on a dynamic updates and traversals of a scene graph whereby cameras automatically attach and detach to the closest object of interest.
This object of interest is then used as a new coordinate origin and, thus, automatically ensures the highest possible numerical precision for salient objects.
This enables an easy integration into existing scene graph implementations.

The dynamic scene graph has been implemented in the open source softawre project \emph{OpenSpace}~\cite{github16openspace}.
OpenSpace has the goal of interactively visualizing and contextualizing a range of astrophysical data, including the most current updates and even concurrent visualization of captured and simulated data.
The software framework is primarily designed for immersive environments, such as dome theaters of planetariums, and is targeting public engagement in science.
Examples of data that can be simultaneously visualized covers a wide range from sub-atomic particle simulations, via micrometer-scale discoveries made by rovers the Martian surface, to volumetric simulations of entire galaxies.

Many state-of-the-art theaters and planetariums support stereoscopic viewing and recently the interest in immersive visualization in VR environments has increased.
Thus softwares needs to support stereoscopic rendering and the ability to render on multi-pipeline systems and complex display configurations.
This poses yet another challenge when dealing with multiple scales as the eye separation needs to be adjusted to the current scale of interest.
Our dynamic scene graph approach offers a seamless solution to this problem. 

It should be noted that our dynamic scene graph is applicable to any visualization tool that uses scene graphs to represent objects, and is compatible with techniques for volumetric rendering as well as order-independent transparency techniques.
In summary, our contributions include:

\begin{itemize}
\item A \emph{Dynamic Scene Graph} approach for representing objects and cameras to avoid precision-related rendering artifacts.
\item A general method for rendering objects with a wide depth range and precision without the explicit need for near and far planes.
\item A scheme for seamless adaption of the eye-separation used for stereoscopic rendering.
\item An analysis of floating point precision errors using interval arithmatic.
\end{itemize}

The paper is structured as follows: After describing the related work in the following section, we provide a theoretical background for the sources of floating point inaccuracies in Section~\ref{sec:theoretical}, followed by a description of our proposed dynamic scene graph framework in Section~\ref{sec:method} and its results in Section~\ref{sec:result}.

\section{Related work}
Computer graphics and interactive visualization have proven to be immensely valuable tools for communicating scientific findings and contextualizing information.
Notable examples of applications where huge scale differences are visualized together include movie productions, such as \emph{Powers of Ten} by Eames~\cite{powersOfTen} from 1977, as well as interactive software for digital planetariums, such as Uniview~\cite{KHECY10}, DigiStar~\cite{ES16} or DigitalSky~\cite{Sky16}, which are based on curated datasets containing information about our universe~\cite{abbott2006digital}. Virtual reality environments such as planetariums~\cite{magnor2010progress, liu20013} have always been in the focus of educators, but a shift towards supporting data inspection on consumer grade hardware is ongoing~\cite{nakasone2009astrosim}.

Fu~\etal~\cite{fu2006navigation} separated the problem of large scale navigation into two categories.
\emph{Techniques for travel} place constrains on user input with the goal of supporting the travel towards and already selected target, whereas techniques for wayfinding support the user in selecting a target by providing a spatial context.
Our travel component is based on the \emph{Spatial Scaling Navigation Model} described in this work operating on a logarithmic camera model in which the navigation speed is dependent on the distance to the object of interest.
In terms of \emph{Techniques for wayfinding}, Li~\etal~\cite{li2006scalable} introduce a world-in-miniature techniques based on logarithmic landmarks for navigation support in astronomical data.
Combining this with the concept of power cubes, described in earlier works~ \cite{fu2006navigation}, allows the creation of paths in this logarithmic space to aid in the large-scale navigation.

The concept of \emph{Power Scaled Coordinates} or \emph{Power Homogeneous Coordinates}, introduced by Hanson~\etal , is a technique introduced to address the travel challenge when creating animations with large scale differences~\cite{hanson2000very}.
The method uses four-dimensional floating point vectors where the first three components determine the direction of the vector, and the fourth encodes a scaling coefficient as a power of ten.
The authors successfully utilize to create a powers-of-ten animation flythrough of the universe in which increasingly larger objects are shown with increasing distance to the Earth.
This elegant solution is viable for scenes where the required precision decreases with distance from the origin, as it does not solve the problem of catastrophic cancellation and thus prohibits high-precision regions in large distances from the origin.
%The proposed system can operate with floating point numbers close to unit scale, which solves the issue of exponent overflow that occurs when performing operations on large and close-to-zero vectors.
%However, the challenge of representing multiple disjoint regions in which high precision is required a great distance from the coordinate origin is not addressed in this work.

Fu~\etal~\cite{fu2007transparently} introduce a depth buffer remapping to Power Scales Coordinates to cover a wider range of distances than what is possible with a fixed point depth buffer and a conventional near and far plane.
The depth buffer range is divided into three regions where small and huge depth values are remapped using logarithmic scales and mid-range values are mapped linearly.
Their method 
While this method may be beneficial for some specific type of scene content, the method does not provide a generic way to select appropriate threshold values, and the choice of three separate regions with different mapping does not necessarily provide better results than one single logarithmic mapping.

The \emph{ScaleGraph}, introduced by Klashed~\etal \cite{KHECY10} as a part of the Uniview software, is the most similar to our proposed framework.
It employs a scene graph structure with sub-scenes to which the camera can attach.
The content of the current sub-scene is rendered in its local coordinate system, maintaining high precision and allowing multiple regions to be represented with high relative precision.
When rendering content outside the current sub-scene, objects are translated onto the bounding sphere of the scene and scaled to compensate for perspective effects.
This translation, however, leads to inconsistencies in stereoscopic rendering when switching scenes as objects jump from the bounding sphere of a scene to its physical location. 
%Vectors between sub-scenes are computed by traversing the scene graph through the closest common ancestor in the tree, to avoid introducing precision errors caused by handling unnecessarily large numbers.
%By moving objects onto the bounding sphere of the current scene, object depths are mapped to a manageable range.
Furthermore, the method does not provide a general solution for depth sorting objects that are outside the current sub-scene.
%Furthermore, the work does not address stereoscopic rendering of objects across sub-scenes.
The traversal of sub-scenes in this work formed the bases for our algorithm of the dynamic traversal of a scene graph.

\section{Theoretical motivation}\label{sec:theoretical}
The co-evolution of graphics hardware, software, and content, largely pushed forward by
the gaming industry, has led to modern graphics processing units that are optimized for
operating efficiently on floating point numbers, vectors, and matrices.
In order to understand the precision and range limitations of the standard computer graphics pipeline used in visualization, we need to base our reasoning on properties on floating point numbers and the operations that we perform on them.

\begin{figure}
\centering
\includegraphics[width=\linewidth]{figure/degradation-circles.pdf}
\caption{The problem of coordinate precision for vertices far from the origin is amplified by limiting the number of mantissa bits to 5, when visualizing a system of celestial bodies represented in a common coordinate system, analogous to a world coordinate system in a conventional scnene graph. }
\label{fig:degradation-circles}
\end{figure}

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing 32-bit floating point numbers \cite{zuras2008ieee} enables the possibility to express numbers in a much larger range than what is possible using 32-bit integers. While a 32-bit signed integer type can only represent numbers in the range $[-2^{31},\, 2^{31} - 1]$, an IEEE754 single precision floating point number approximately supports the range $[-2^{127},\, 2^{127}]$.

In floating point data types, values are represented as a sign, a mantissa, and an exponent. This representation has the attributes of resulting in high precision close to zero, decreasing precision for larger numbers, and a dramatic increase in value range. The IEEE754 single precision floating point format uses 1 sign bit, 8 exponent bits, and 23 mantissa bits. Equation \ref{eq:floatingpoint} yields the value $v$ represented by the sign bit $s$, the mantissa bits $\mathbf{m}_i$, and the integer $e \in [-127, 128]$ which is encoded in the exponent bits. 

\begin{equation} \label{eq:floatingpoint}
v = \begin{cases}
(-1)^s \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i} 2^{-126}, & \text{for } e = -127 \\
(-1)^s (1 + \sum_{i = 1}^{23}\mathbf{m}_i\,2^{-i}) 2^e, & \text{for } -126 \leq e \leq 127 \\
\text{Special numbers such as NaN or }\infty, & \text{for } e = 128
\end{cases}
\end{equation}

\noindent Only values that can be composed by this set of bits are valid 32-bit floating point numbers. If exponent of a number is not in the range $[-126, 127]$ it will be represented as $\infty$ or $-\infty$. One critical characteristic of valid IEEE754 floating point numbers $\mathds{R}_f$ \alexcomment{Is there a better symbol for this?} is their inability to represent all possible numbers $\mathds{R}$. All numbers $\mathds{R} \setminus \mathds{R}_f$ that are not representable are rounded to their closest representable number in $\mathds{R}_f$. This characteristic is at the heart of the problem of floating point inaccuracies.

An alternative data type for representing numbers is the IEEE754 64-bit floating-point format (double precision). The standard uses 1 sign bit, 11 exponent bits, and 52 bits for the mantissa. Using 128-bits (quadruple precision), it is possible to further increase the precision and exponent range. Increasing the bit depth will naturally require more computational resources, and might lack common hardware support, thus making their usage unfeasible for real-time visualization.

\subsubsection{Exponent underflow and overflow}
The exponent range $[-126, 127]$ in 32-bit IEEE754 floats provides a possible scale range of approximately 76 orders of magnitude (OM).
Computations that yield exponents outside that range will cause exponent underflow or overflow and result in loss of data.
While this range is larger than the 60 OM of scales present in the known cosmos, problems may arrise in situations such as when distances need to be squared which may generate numbers in a range of 120 OM.
This problem can be mitigated using a logarithmic representation as done in Power Scaled Coordinates or by using IEEE754 doubles, which have the exponent range $[-1022, 1023]$ resulting in representablity of approximately 616 OM. \emilcomment{Maybe add something about length calculation by division of largest component, to show that double and psc is not necessary.}

%The scale difference between the Planck length and the diameter of the observable universe is approximately $62$ OM. While 32-bit floats are able to represent this scale difference, a conventional approach to computing lengths and normalizing vectors would break down.

\subsubsection{Precision limitations} \label{sec:theory:pl}
Due to the limited number of mantissa bits, values need to be rounded in order to be represented as floating point numbers. An upper bound of the absolute spacing between a floating point number $v$ and adjacent numbers can be expressed as $\epsilon|v|$\cite{higham2002accuracy}, where the \emph{machine epsilon} $\epsilon$ is a constant given by $2^{-(n + 1)}$ where $n$ is the number of mantissa bits. Hence, the maximum error introduced by rounding of $v$ can be written as $u|v|$, with the rounding error coefficient $u = \frac{\epsilon}{2}$.
For IEEE754 float, $\epsilon$ is $2^{-24}\,\approx
\, 5.96 \cdot 10^{-8}$, meaning that values are precise to about 7 significant figures in decimal notation.

Double precision floating point numbers have a machine epsilon of $\epsilon = 2^{-53}\approx 1.11 \cdot 10^{-16}$. In decimal notation, this corresponds to a precision of 15 significant figures. For quadruple precision the corresponding number is 34 significant figures.

In a computer graphics pipeline, the limitation posed by the machine epsilon imposes that vertices are represented with less precision the further way from the coordinate system origin they are. Figure~\ref{fig:degradation-circles} shows a simulation where the number of mantissa bits has been set to 5, causing $\epsilon = 2^{-6} = \frac{1}{64}$. In a real world example using IEEE754 floats, at Pluto's distance from the Sun (about $5.102 \cdot 10^9 \, \text{km}$) the distance between two adjacent floating point numbers is approximately $5.102 \cdot 10^9 \cdot 2^{-24} \, \text{km} = 304.1 \, \text{km}$, which is equivalent to about an eighth of Pluto's diameter ($2374 \, \text{km}$). When visualized, this leads results such the one shown in Figure~\ref{fig:res:plutocomp}. By using double precision, the corresponding distance between floating point numbers on Pluto becomes $5.102 \cdot 10^9 \cdot 2^{-53} \, \text{km} = \SI{56.64}{\micro\metre}$. Even at this relatively high resolution it is impossible to represent and contextualize tiny structures such as molecules. \emilcomment{Do we need to motivate this better? Who would ever want to look at molecules at Pluto?}

\begin{figure}
\centering
\fbox{\includegraphics[width=\linewidth]{figure/precision.png}}
\caption{Comparison rendering of Pluto. Using Power Scaled Coordinates, 9 quantization levels of floating point numbers are visible at Pluto's distance of 5.1 billion km from the Sun. Our method achieves high fidelity renderings while still representing objects with single precision floating point numbers. }
\label{fig:res:plutocomp}
\end{figure}


%Pluto number of floats: (Diameter / Distance) / Machine Epsilon
%Distance: $5102004914.623568$\,km
%Diameter: about $2374$\,km
%Machine Epsilon: 5.96e-08
%Pluto epsilon: $(2374 / 5102004914.623568) / 5.96e-08 = 7.807169030929783$


\subsection{Interval arithmetic}
In order to reason about the precision of the different floating point operations, it is possible to use interval arithmetic as a mathematical tool to track potential sources of rounding error~\cite{Hickey2001}. Due to the upper bound of the rounding error, a value $v$ may be rounded to a number $v' \in [v - u|v|, v + u|v|]$. Using interval arithmetic, the interval can be rewritten as $v(1 + u[-1,1])$.

For the remainder of the following sections, $\oplus$, $\ominus$, $\odot$, and $\oslash$ will be used to denote floating point addition, subtraction, multiplication, and division respectively. The IEEE754 standard requires that the result of each of these operations is identical to the result of an infinitely precise computation followed by a rounding to the closest representable float. The operators are commutative and yield the possible output intervals given two real number intervals $[x] = [x_1, x_2]$ and $[y] = [y_1, y_2]$ as described in Equation \ref{eq:floatoperations}.

\begin{equation} \label{eq:floatoperations}
\begin{split}
[x] \oplus [y] & \subseteq ([x] + [y])(1 + u[-1, 1]) \\
[x] \ominus [y] & \subseteq ([x] - [y])(1 + u[-1, 1]) \\
[x] \odot [y] & \subseteq [x][y](1 + u[-1, 1]) \\
[x] \oslash [y] & \subseteq \frac{[x]}{[y]}(1 + u[-1, 1]) \\
\end{split}
\end{equation}
 
\noindent For operations involving the degenerate intervals $[0, 0] = \{0\}$ and $[1, 1] = \{1\}$, we observe some special cases where it is possible to make stronger claims about the output intervals:
\begin{equation} \label{eq:floatspecialcases}
\begin{split}
[x] \odot \{1\} & = [x] \\
[x] \oplus \{0\} & = [x]
\end{split}
\end{equation}

\subsection{Propagation of error intervals}
When a result from one floating point operation is used in further computations, rounding errors will propagate and compound through the whole series of operations. This, for example, may lead to scenarios where expressions on the form $(a + b) - b$ may evaluate to $0$ for a sufficiently small $a$ and large $b$. This phenomena is known as 
\emph{catastrophic cancellation}~\cite{Cuyt2001}. In any rendering pipeline where, for example, matrix multiplications or vector additions result in this type of operations there is a risk for floating point precision problems.

Let $\mathds{E}$ be the set of intervals that can be written in the form $cu[-1, 1]$, where c is a non-negative constant that is not a function of any floating point value. For any intervals $[e_1], [e_2] \in \mathds{E}$, there are intervals $[e_3], [e_4] \in \mathds{E}$, such that
$[e_1]+[e_2] \subseteq [e_3]$ and $[e_1e_2] \subseteq [e_4]$. 

We let $[e_i] = c_iu[-1, 1] \in \mathds{E}$ and consider two intervals $a(1 + [e_1])$ and $b(1 + [e_2])$. First, we study the effects of floating point multiplication acting on these intervals.

\begin{equation} \label{eq:errortermmult} 
\begin{split}
a(1 + [e_1]) \odot b(1 + [e_2]) \\
\subseteq ab(1 + [e_1])(1 + [e_2])(1 + u[-1, 1]) \\
\subseteq ab(1 + [e_3])
\end{split}
\end{equation}

The output interval in Equation \ref{eq:errortermmult} has the size $|abc_3u|$, meaning that the maximum absolute rounding error of floating point multiplication is proportional to the absolute value of the product $ab$. However, the relative rounding error $c_3u$ is independent of these factors. We proceed with studying the properties of floating point addition.

\begin{equation} \label{eq:errortermadd} 
\begin{split}
a(1 + [e_1]) \oplus b( 1 + [e_2]) \\
\subseteq \big(a(1 + [e_1]) + b( 1 + [e_2])\big)(1 + u[-1, 1]) \\
\subseteq a(1 + [e_4]) + b(1 + [e_5])
\end{split}
\end{equation}

\noindent Equation \ref{eq:errortermadd} yields a maximum rounding error of $|ac_4u| + |bc_5u|$. The relative error of the floating point sum depends on the absolute value of the individual terms. By applying the equation iteratively, we see that for floating point sums $\osum_i [x_i] = [x_1] \oplus [x_2] \oplus ... \oplus [x_n]$, with $[x_i] = x_i(1 + [e_i])$ Equation~\ref{eq:errortermsum} holds regardless of the order of operations.

\begin{equation} \label{eq:errortermsum} 
\osum_i [x_i] \subseteq \sum_i [x_i](1 + [e'_i])
\end{equation}

Floating point sums will accumulate errors from all contributing terms, creating a risk for catastrophic cancellation if both large positive and negative terms are involved in a computation with relatively small expected result. 

Given a matrix $X$ with components $X_{ij}$ and a matrix $[E]$ with interval components $[E_{ij}]$, let $[X^E]$ denote a matrix with the interval components $X_{ij}(1 + [E_{ij}])$. We use the operator $\otimes$ to denote matrix multiplication, in which output components are computed as sums of products. By combining Equations~\ref{eq:errortermmult} and~\ref{eq:errortermsum}, we get Equation~\ref{eq:matrixmult} where all interval matrix components $[E_{ij}], [F_{ij}] \in \mathds{E}$ and $e_{ijk} \in \mathds{E}$. The number of terms $m$ equals the number of columns in $A$ as well as the number of rows in $B$.

\begin{equation} \label{eq:matrixmult} 
\begin{split}
([A^E] \otimes [B^F])_{ij} \subseteq \osum_{k = 1}^m A_{ik} (1 + [e_{ij}])\odot B_{kj} (1 + [F_{kj}]) \\
\subseteq \sum_{k = 1}^mA_{ik} B_{kj}(1 + [e_{ijk}])
\end{split}
\end{equation}

\noindent We observe that the maximum floating point error associated with a component $(AB)_{ij}$ of a matrix product is given by $\sum_{k = 1}^m|A_{ik} B_{kj} [e_{ijk}]|$. Using the special cases from Equation \ref{eq:floatspecialcases}, we note that $e_{ijk} = 0$ if $A_{ik} = 0$ or $A_{ik} = 0$. Depending on the structure of the matrices $A$ and $B$, the output component may be subject to catastrophic cancellation. 

With these quantifications of floating point errors introduced by scalar operations and vector multiplications, we can identify the components in the graphics pipeline that are prone to introduce visible precision problems.

\subsection{Precision-related root causes of rendering artifacts}
%Having observed how error intervals originate and propagate through various floating point operations, we can studying the operations performed in the graphics pipeline to identify sources of precision problems of vertex as well as fragment operations.




%\subsubsection{Vertex transformations}
%\alexcomment{I think we should cut a bit on this part. It's a bit too long for the value that it has. It is a visualization conference, so we can expect people to know all of this}
%In computer graphics, content is commonly organized in a \emph{scene graph} that allows coordinate transformations to be represented hierarchically. Each node in the graph may contain a transformation, represented as a $4\times 4$ matrix, that affects all children. Objects consist of vertices $\mathbf{x}$ represented as homogeneous coordinates $(x, y, z, w)^t$, where $x$, $y$, and $z$ are the coordinates in a local \emph{model coordinate system} and $w = 1$. In order to transform a vertex to a global \emph{world coordinate system}, the transformation matrices of the ancestor nodes are concatenated to a final \emph{model matrix} using matrix multiplication as described above. This final matrix is then applied to the vertex $\mathbf{x}$ to acquire the world coordinates $\mathbf{w}$.

%A \emph{view matrix} is usually generated using a camera position and rotation expressed in the world coordinate system and then used to transform world coordinates to \emph{view coordinates}, with the camera in the origin, and the z-axis parallel to the view direction. This view matrix is multiplied with the world coordinate $\mathbf{w}$ and outputs the view coordinates $\mathbf{v}$.

%View coordinates are transformed into \emph{clip coordinates} $\mathbf{c}$ using a \emph{projection matrix}, in this case a perspective projection matrix.
%After that, content that does not satisfy the equation $-w \le x, y, z \le w$ will be subject to frustum culling, and will be discarded from rendering. The vector is divided by $w$ to compute the \emph{normalized device coordinates (NDC)}. This last step, commonly called \emph{perspective division}, causes objects farther away from the camera to appear smaller on the screen. The $x$ and $y$ in NDC determines the object's position on the screen, with $(-1, -1)$ representing the bottom left corner and $(1, 1)$ upper right one.

%Depth sorting is commonly performed using a depth buffer, which stores a depth value for each pixel on the screen. Fragments are only rendered to if they are closer than the closest already rendered fragment at the same screen location. The depth value of a fragment is derived from the $z$-component of the normalized device coordinates using a linear mapping. Commonly, $z = 0$ represents the \emph{near plane} and and $z = -1$ represents the \emph{far plane}. If the depth buffer is not represented with 32-bit floating point numbers, the depth is converted to the appropriate type. 

%\subsubsection{Shading operations}
%\alexcomment{Move this to later}
%Shading models frequently use various coordinate vectors of objects in the scene, such as light sources, surface normals and the camera position to compute fragment colors. Various shading algorithms may require transformations of vectors between the model coordinate system, the world coordinate system, the view coordinate system, or some other coordinate space. These transformations are commonly implemented as matrix multiplications.

%\subsection{Rendering artifacts}
%\alexcomment{Merge this into 3.4, replacing most of 3.4 content and leaving a paragraph explaining the near/far plane}
Lack of precision may give rise to several types of visual artifacts. If errors in the $x$ and $y$ components of the normalized device coordinates are in the same order as the size of the pixels on the screen, there will be a visible displacement of vertices. If the rounding error of $z$ when stored in the depth buffer is larger than the $z$ difference of any two fragments covering the same pixel, there is a risk for z-fighting, i.e. that the incorrect fragment is rendered on top. Rounding errors introduced in shading operations may result in in poor color precision.

In the following sections, we study the precision error implications of the coordinate operations in the conventional graphics pipeline to determine the set of circumstances in which precision problems may be introduced. By using interval arithmetic to trace the errors in $x$ and $y$ it is possible to predict vertex displacement issues. Similarly, the accumulated error interval of the z-component can be used to determine when there is a risk for z-fighting. By studying the effect of transformation matrices acting on coordinate vectors, we can also develop a means to identify the risk of precision loss in various shading operations.

In computer graphics, content is commonly organized in a \emph{scene graph} that allows coordinate transformations to be represented hierarchically. Each node in the graph may contain transformations affecting all children. In order to transform an object to a global \emph{world coordinate system}, the transformation matrices of the ancestor nodes are concatenated to a final \emph{model matrix} using floating point matrix multiplications. Subsequently, world positions are transformed into view space, through the use of a \emph{view matrix} and then mapped onto the screen using a \emph{projection matrix} and \emph{perspective division}.

Using column vectors, the interval vector $[\mathbf{n}]$ of possible normalized device coordinates is derived from the model coordinates $[\mathbf{x}]$ in Equation \ref{eq:mvp}.

\begin{equation} \label{eq:mvp}
\begin{split}
[\mathbf{c}] = [P]\otimes [V] \otimes [M] \otimes [\mathbf{x}] \\
[\mathbf{n}] = [\mathbf{c}] \oslash [\mathbf{c}_w]
\end{split}
\end{equation}
 
Here, $[P]$ is the projection matrix, $[V]$ is the view matrix, $[M]$ is the model matrix, and $[\mathbf{x}]$, $[\mathbf{c}]$ and $[\mathbf{n}]$ are interval vectors on the form $([x], [y], [z], [w])^t$. The error interval size of the normalized device coordinates depends on the collective effects of $[M]$, $[V]$, $[P]$, and the perspective division as they act on $[\mathbf{x}]$. 

$M$ and $V$ are commonly composed from three types of transformations that are encoded in the scene graph: scaling, translation, and rotation. We study the effects of applying scaling, translation, and rotation on a generalized interval model matrix or view matrix $[A]$, composed of a translation vector $\mathbf{[a]}$ and a combined rotation/scaling 3x3-matrix $[A']$, as given by Equation \ref{eq:matrixa}. Scaling, rotation and translation matrices can all be written on the same form as $[A]$. To observe how the matrices act on a coordinate vector we can study the fourth column in $[A]$.
\begin{equation} \label{eq:matrixa}
[A] = \mleft(
\begin{array}{c|c}
  [A'] & [\mathbf{a}]\\
  \hline
  \{\mathbf{0}\} & \{1\} 
\end{array}
\mright)
\end{equation}

After evaluating the possible effects of the scaling, translation, and rotation we continue with a similar analysis for the projection matrix, the perspective division, and the mapping to a depth buffer representation.

\subsubsection{Scaling}
Consider an interval 4x4-matrix $[S]$ composed of the 3x3 interval matrix $[S']$, interval vector $\mathbf{[s]}$ and a bottom row of degenerate intervals analogously to the matrix A. Let the components of $[S]$ be scaling coefficient intervals, and let all other components of $[S]$ and $\mathbf{[s]}$ equal the degenerate interval $[0, 0]$. Equation \ref{eq:scalingconclusion} expresses the components of the output matrix $\hat{S}$, which is also a matrix on the form of A.

\begin{align} \label{eq:scalingconclusion}
[\hat{S}] = ([S'] \otimes [A'])_{ij} &\subseteq [S'_{ii}]\,[A'_{ij}](1 + [e_{ij}]) \\
[\hat{\mathbf{s}}] = ([S'] \otimes [\mathbf{a}])_{i} &\subseteq [S'_{ii}]\,[\mathbf{a}_i](1 + [e_{j}])
\end{align}

The rounding error introduced by an interval scaling matrix $[S]$ is proportional to the size of the scaling factors in $[S']$.  Any error interval in $A'$ and $\mathbf{a}$ will be scaled proportionally. Thus, we conclude that applying a scaling matrix to a matrix $A$ \emph{does not cause a significant loss of relative precision in any matrix component}.  

\subsubsection{Translation} \label{sec:theory:translation}
A corresponding analysis is made for translation matrices, composed out of $[T']$ and $[\mathbf{t}]$, with degernate intervals $[T'_{ij}] = [I_{ij}, I_{ij}]$, where $I$ is the identity matrix. The components of $[\mathbf{t}]$ representing a translation interval vector.

\begin{align} \label{eq:translationconclusion}
[\hat{T}_{ij}] = ([T'] \otimes [A'])_{ij} = [A'_{ij}]\\
[\hat{\mathbf{t}}_i] = [\mathbf{t}_i] \oplus [\mathbf{a}_i] \subseteq ([\mathbf{t}_i] + [\mathbf{a}_i])(1 + [e_i])
\end{align}

\noindent We note that the precision in $\hat{T}_{ij}$ is preserved from $A'$ when any translation matrix $T$ is applied. However, any error intervals encoded in $[\mathbf{t}_i]$ and $[\mathbf{a}_i]$ will propagate to $\hat{\mathbf[t]}_i$, and will \emph{not} be scaled down even if $|[\hat{\mathbf{t}}_i]|$ is orders of magnitude smaller than $|[\mathbf{t}_i]|$ and $|[\mathbf{a}_i]|$. This scenario \emph{may lead to catastrophic cancellation} in the component $[\hat{\mathbf{t}}_i]$.

\subsubsection{Rotation} \label{sec:theory:rotation}
Using $[R]$ to denote a rotation interval matrix, composed of the upper left 3x3 matrix $[R']$ and a upper right 3-dimensional column vector $\mathbf{r}$, with the interval components $[R'_{ij}] \subseteq [-1, 1]$, and $\mathbf{r}_i = [0, 0]$, we study the effect of multiplying $[R]$ with the interval matrix $[A]$.

\begin{align} \label{eq:rotationconclusion}
\hat{R}'_{ij} = (R' \otimes A')_{ij} &\subseteq \sum_{k = 1}^3 R'_{ik}\,A'_{kj}(1 + [e_{ijk}]) \\
\hat{\mathbf{r}}_i = (R' \otimes \mathbf{a})_{i} &\subseteq \sum_{k = 1}^3 R'_{ik}\,\mathbf{a}_k(1 + [e_{ijk}])
\end{align}

The potential rounding error of any output component $A_{ij}$ will be equal to $\sum_{k = 1}^3 |[R'_{ik}]\,[A'_{kj}][e_{ijk}]|$. Given the fact that the intervals in $[R]$ are confined to $[-1, 1]$, we see that the maximum possible precision loss in any matrix component is governed by the values of $A$. When applying $[R]$ on a matrix or vector with both large and close-to-zero components, the precision in the small components may be lost. However, the loss of precision in the most significant matrix components will be minimal. This means that when using $[R]$ to transform the coordinate vector $[\mathbf{a}] = ([x], [y], [z])^t \subseteq [\mathbf{\hat{a}}][|\mathbf{a}|]$, neither the precision of the direction $[\mathbf{\hat{a}}]$ nor the magnitude $[|\mathbf{a}|]$ will be affected significantly. The same is true for the column vectors in $[A']$, which means that the significant properties of $[A']$ are preserved in $[\hat{R}']$.

\subsubsection{Perspective and depth}
When a typical perspective matrix $P$ is multiplied with a homogeneous view coordinate vector $\mathbf{v} = (x, y, z, 1)^t$, the sums of products to compute the output x, y, and w components will only consist of one non-zero term. Hence, there is no significant loss of relative precision in these components.

Furthermore, while the perspective division shrinks the projection image of distant objects, this also has the effect of scaling down any floating point precision errors present in distant vertex coordinates. Hence, the sensitivity of visible vertex displacement of a vertex is inversely proportional to the depth of the vertex in view space.

The z-component in clip space $c_z = (P\mathbf{v})_z$ is given by Equation \ref{eq:nearfar}, where n and f are the distance from the camera to the near and far plane respectively.

\begin{equation} \label{eq:nearfar}
\lambda_1 = -\frac{f + n}{f - n}, \; \lambda_2 = \frac{2fn}{f - n}, \; [\mathbf{c}_z] = [\lambda_1] \otimes [\mathbf{v}_z] \oplus \lambda_2
\end{equation}

\noindent The result is an affine mapping of depths such that vertices in the near plane yield $\mathbf{c}_z = 0$ and vertices in the far plane give $\mathbf{c}_z = \mathbf{c}_w$.
Subsequently, the perspective division maps $[\mathbf{n}_z]$ into the range $[0, 1]$.
If an integer depth buffer is used, this non-linear mapping results in a higher depth resolution for close objects than for distant ones. Setting $n$ to a small number may cause distant vertices to collapse to the same $\mathbf{n}_z$ and cause z-fighting. 

The problems of depth buffer precision are well known and have been subject to thorough analysis \cite{cozzi20113d}. Upchurch and Desbrun~\cite{upchurch2012tightening} show that good depth resolution can be maintained even with a far plane placed in $\infty$. However, precision problems for distant objects remain an issue for small $n$.

\subsection{Sources of precision errors}
To summarize this section, we have seen that:
\begin{itemize}
\item Large translation matrices acting on small vectors or translations may lead to significant loss of precision.
\item Rotation and scaling matrices do not induce significant precision loss when applied to vectors or other matrices 
\item Perspective division scales down the floating point error intervals in vertex coordinates for distant objects.
\item Close near planes cause poor depth precision for distant objects.
\end{itemize}

\section{Method}\label{sec:method}
\begin{figure}
\includegraphics[width=\linewidth]{figure/scenegraph.pdf}
\caption{A potential scene graph consisting of two translation nodes for the solar system and the Earth system (1,3) and three rendering nodes for the Sun (2), Earth (4), and the Moon (5). Cameras (A, B) can be attached to different nodes, thus avoiding the introduction of large, error-prone translation values. (a) shows the nodes relative location, whereas (b) shows their organization in a graph. The arrows represent local upwards (red) and downwards (blue) transformations.}
\label{fig:scenegraph}
\end{figure}

After identifying the sources of potential floating point precision inaccuracies in the computer graphics pipeline, we can now elaborate on our proposed framework.
The \emph{Dynamic Scene Graph} (DSG) utilizes these insights such that it becomes possible to achieve a high-fidelity rendering throughout the scene graph independent of the location of the coordinate system origin.
This is achieved by utilizing a \emph{dynamic camera attachment} and a \emph{relative scene traversal} performed with respect to a special node; the attachment node (AN).
First, we describe the relative scene traversal assuming a static camera, and then in the following section describe the attachment changes of moving cameras.

The DSG is based on a traditional scene graph structure, but instead of a regular traversal order that starts at the root node of the graph downward, we propose a traversal order that starts at the AN and can traverse both upwards and downwards.
Each node in the scene graph can contain a \emph{transformation}, consisting of a \emph{translation}, \emph{rotation}, and \emph{scaling}, which are all described relative to a node's parent.
In addition, nodes may contain a visual object representation, for example a planet or a spacecraft, specified in the node's local coordinate system.
If a scene graph node contains a visible object, we refer to it as a \emph{rendering node}, otherwise it is a \emph{transformation node}.
Unlike traditional scene graphs, each scene graph node has a radial \emph{extent}, which determines the node's sphere of influence. each scene graph node's extent has to cover at least all the children's spheres of influence.
% Specify limitation of the graph? Or later?
%The location and sizes of the scene graph nodes have to be specified such that 

Figure~\ref{fig:scenegraph} shows a possible scene graph with 5 nodes with their extents; (1,3) are transformation nodes centered on the solar system and the Earth barycenter respectively, (2, 4, 5) are rendering nodes containing the surface of the Sun, the Earth, and the Moon. Two cameras (A, B) are attached to the scene at different attachment nodes.

\subsection{Dynamic Scene Graph Traversal} \label{sec:method:dsgt}
As described in the previous section, the camera is attached to a specific scene graph node \texttt{N}.
Rendering, or otherwise accessing, the contents of the scene graph requires a traversal, which in a traditional scene graph is started at the root node and traverses through the child nodes.
In our proposed framework, this traversal starts at the attached node \texttt{N} instead with the ability of moving up and down in the scene graph with the ability of inverting the involved relative transformations..

The traversal of the scene graph works as follows: For each scene graph node \texttt{M}, the shortest path between \texttt{M} and the currently attached node \texttt{N} is computed in the graph.
Figure~\ref{fig:scenegraph}~(b) shows two examples of this with the camera being attached to node \texttt{3} and node \texttt{5} respectively.
For each step along this path, the transformation matrices are concatenated.
If the transition is performed downwards towards the leaf nodes (blue arrows in Figure~\ref{fig:scenegraph}~(b)), the transformation matrices are concatenated analogous to traditional scene graphs.
If the transition is performed up towards the root node of the graph (red arrows in Figure~\ref{fig:scenegraph}~(b)), the transformation is inverted before concatenated.
The final transformation matrix is the applied to the scene graph node \texttt{M}.

As an example, we use $v_{i,j}$ as a notation to denote the transformation from node \texttt{i} to \texttt{j} and $v{j_i} = -v_{i,j}$ as the inverse of this transformation in the scene graph depicted in Figure~\ref{fig:scenegraph}~(b).
$v_{i,i}$ specifies the local translation that the scene graph node \texttt{i} uses.
For camera \texttt{A}, the rendering of the Sun (node \texttt{2}) is performed using the transformations $v_{2,1} \cdot v_{1,3} \cdot v_{3,3}$.
The rendering of the Earth (node \texttt{4}) however, is performed only with the translation $v_{4,3} \cdot v_{3,3}$, ignoring all other scene graph nodes and thus not being affected by their values.
For camera \texttt{B}, the transformation for the Sun are $v_{2,1} \cdot v_{1,3} \cdot v_{3,5} \cdot v_{5,5}$ and for the Earth $v_{4,3} \cdot v_{3,5} \cdot v_{5,5}$.

First, this means that when different cameras are present in the scene graph, each camera might have its own traversal path.
More importantly, this means that for two nodes \texttt{N} and \texttt{M} only the subtree with the closest common parent at the root needs to be traversed in order to retrieve all information necessary to transform \texttt{M} into the local coordinate system of \texttt{N} and vice versa.
Without the requirement of including the root node in every traversal, the location of the local subtree does not have an impact on the precision of the objects contained within thus evading the potential of catastrophic cancellation that are stated in Section~\ref{sec:theory:translation}.
This is at the heart of our proposed method as it leads to a high-precision rendering independent of the location of the nodes within the extended scene graph.
By utilizing the shortest path between \texttt{M} and \texttt{N} we avoid large the translations which would otherwise be necessary if the all transformations originate from the root node and which would lead to precision problems.

This technique enables the highest precision for \texttt{N} where precision decreases with growing distance from the center of \texttt{N}.
This is based on the observation that high-frequency changes are only visible in objects that are close to the camera and thus cover more pixel on the screen.
As described in Section~\ref{sec:theoretical}, the representable precision decreases with increasing numbers, such that small details in large objects with great distances would become prone to precision error.
However, projected changes that are smaller than a fraction of a pixel will not be visible and, thus, do not need to be represented in the first place.
Conceptually, this pivots the scene graph in such a way that the node \texttt{N} is placed at the root, all necessary transformation inversions are performed , and a traditional traversal algorithm is used.

\subsection{Dynamic camera attachment} \label{sec:method:dca}
In our scene graph, a camera is always attached to a single node, the AN, and the camera's position and orientiation is expressed in AN's coordinate system.
At all times, the AN is the deepest node in the scene graph whose sphere of influence fully encompasses the camera position.
In Figure~\ref{fig:scenegraph} camera \texttt{A} is located in nodes \texttt{1} and \texttt{3}, and is thus attached to node \texttt{3}. Likewise, camera \texttt{B} is inside nodes \texttt{1}, \texttt{2} and \texttt{5} and is thus attached to node \texttt{5}.

Figure~\ref{fig:imagesequence} shows an example of the dynamic camera attachment in a flight from the surface of Earth to Saturn's moon Titan. The camera changes its AN four times on this transition.

There are two cases for updating a camera's AN in response to a user's interaction.
If a camera enters the sphere of influence of a current AN's child, the child becomes the new AN the camera thus moves down the scene graph.
If the camera leaves the sphere of influence of the current AN, the AN's parent becomes the new AN and the camera thus moves up the scene graph.
At each step, the distances of the camera to the AN center and the distances to all children are computed and specified \emph{relative to the AN} as opposed to specifying the values relative to the global coordinate system origin.
This circumvents the risk of catastrophic cancellation by ensuring that unnecessarily large vectors are not involved in the computation.
%A camera attaches to the AN's parent node if the distance from the camera to the center of the AN is larger than the AN's extent. 
%Conversely, whenever a camera location enters the sphere of influence of one of the AN's child nodes, the child node becomes the camera's new AN.

\begin{figure}
\centering
\includegraphics[width=\linewidth]{figure/attachment-switch.pdf}
\caption{When the camera attaches to a different node its coordinate system changes. While the old location \texttt{A} is expressed relative to \texttt{N} as \textbf{v}, the new location \texttt{B} is expressed relative to \texttt{M} as \textbf{w} $-$ \textbf{n} instead.}
\label{fig:coordsystems}
\end{figure}

In both transition cases, the location of the camera must change reference frames.
Figure~\ref{fig:coordsystems} shows a camera's old (\texttt{A}) and new (\texttt{B}) location before and after an AN change. 
\texttt{A} is expressed relative to \texttt{N} by the vector $v$, whereas \texttt{B} is expressed relative to \texttt{M} as the vector $\mathbf{w}$.
The old and new locations are related by $\mathbf{w} = \mathbf{v} + \mathbf{n}$.
To allow for this remapping to be computed without precision problems, extents of the scene graph nodes must be chosen such that the ratio between child and parent nodes' extent is significantly larger than the machine epsilon. In order to circumvent rounding errors related to translation as described in \ref{sec:theory:translation}, extents are chosen such that the camera attaches to a child render node before the details of the render node becomes visible.

Similarly to the translation, the rotation of the camera has to be inverted by applying the inverse of the rotation of \texttt{N} towards \texttt{M}.
As stated in Section~\ref{sec:theory:rotation} no significant precision error with the rotation will occur.

\begin{figure*}
\centering
\newcommand{\abWidth}{0.325\linewidth}
\subfigure[Surface of the Earth]{\fbox{\includegraphics[width=\abWidth]{figure/image-sequence-1-2.png}}}
%\hfill
\subfigure[Earth from  orbit]{\fbox{\includegraphics[width=\abWidth]{figure/image-sequence-2-2.png}}}
\subfigure[Saturnian sphere of influence]{\fbox{\includegraphics[width=\abWidth]{figure/image-sequence-3-2.png}}}
\\
\subfigure[Titan with Saturn in the background]{\fbox{\includegraphics[width=\abWidth]{figure/image-sequence-4-2.png}}}
%\hfill
\subfigure[Surface of Titan]{\fbox{\includegraphics[width=\abWidth]{figure/image-sequence-5-2.png}}}
\subfigure[Scene Graph Representation]{\includegraphics[width=\abWidth]{figure/earth-titan-graph.pdf}}

\caption{A journey of 1.65 billion km as sequence of images of a flight started on Earth's surface (a), showing Earth from the view of geostationary satellites (b), entering the Saturnian system (c), entering Titan's sphere of influence (d), and approaching Titan (e). Figure (f) shows the accompanying scene graph structure and and which locations the individual images were taken.}
\label{fig:imagesequence}
\end{figure*}

\subsection{Dynamic object attachment} \label{sec:method:doa}
In addition to the dynamic camera attachment, where the camera can be reattached to children or parent nodes, the same technique is also required for scene graph nodes themselves.
When dealing with dynamic astronomical objects it is possible for distinct sibling objects to approach each other.
An example of this is visualizing the New Horizons mission~\cite{stern2009new}, where the spacecraft launched from Earth and approached both Jupiter and Pluto.
If the scene graph node containing New Horizons would statically be a direct child of the solar system, floating point precision would not suffice for accurately representing distances from the probe at Earth, Jupiter, and Pluto, respectively.
In order to avoid this, we enable scene graph nodes to dynamically attach to different nodes under specialized circumstances, that is, being highly mobile objects such as spacecraft, asteroids, or comets.
In Figure~\ref{fig:teaser}, the New Horizons spacecraft scene graph node has been dynamically attached to the Pluto barycenter node, in order to allow simultanueous high fidelity rendering of the probe as well as the planetary system.







%\subsection{Dynamic Scene Graph}\label{sec:scenegraph}
%Analogous to a traditional 
%In our implementation, the dynamic scene graph, is composed of three different types of nodes:
%\begin{enumerate}
%	\item The root node of the tree representing the whole scene in our 3-d space. It also can be used to represent the center of the universe.
%	\item Internal nodes representing positions, the center of mass of two or more bodies orbiting each other (e.g. the barycenter of a celestial system) or a renderable entity (e.g. a celestial body, spacecraft, etc.).
%	\item The leaf nodes representing a renderable entity or the virtual camera of the system.
%\end{enumerate}
%
%Each node stores also the transformation matrix (which can be composed by rotation, translation and scale matrices as well) to be applied over the element in the node, and a \textbf{node radius}.
%A \textbf{node radius} is defined as a virtual sphere (bounding volume) centered on the node's spatial position which allows one to decide when another node's spatial position is in its influence sphere.
%As usual in a scene graph tree structure, the transformation matrix is inherited by all children of a node.
%
%Figure~\ref{fig:scenegraph} illustrates the schematic configuration of a scene graph (bottom drawings) for the scene in the figure (top drawing).
%In Figure~\ref{fig:scenegraph} the item number 1 represents the node radius of the whole scene; item number 2 is the another node radius in the scene, with items 4 and 5 inside it plus the virtual camera of the system; item 3, 4 and 5 are renderable entities.
%The virtual camera of the system (item \textbf{A}) is attached to the item 4 node.
%This scene graph can be seen as a simplistic representation of a an heliocentric system with the Sun in yellow and Earth in blue orbiting it.
%The Moon in gray is orbiting the Earth.
%
%\joncomment{Emil, do you think is possible to write some text aside the nodes in the bottom part of Figure~\ref{fig:scenegraph}? Like: badycenter, Moon, Sun, etc?} 
%Also in Figure~\ref{fig:scenegraph}, in the bottom right drawing, the representation of the scene as a scene graph tree can be seen.
%In this example the virtual camera is focusing on item 4, the Moon. We call our scene graph a dynamic scene graph.
%
%In order to render the scene correctly, a commonly used scene graph traverses all nodes in the tree composing (by multiplication) \joncomment{concatenating?} the transformation matrices throughout the path from the root node to each of the leaf nodes.
%In this structure the root of the scene graph is frequently seen as the center of coordinates of the scene (the world coordinates).
%Considering the possible differences in size and distance when visualizing objects in universe and the classical way a scene graph is traversed during a scene rendering, this approach can lead to precision problems in the final rendering of the scene, as discussed in section~\ref{sec:theoretical}, mainly because the amount of translation transformations performed between different nodes and the final perspective transformation.
%In this case, a higher scene graph may lead to higher errors.\joncomment{Should we say more about position and depth and catastrophic cancellation?}
%
%%As we noted, traversing the scene graph from root to leaf during each rendering pass will produce a catastrophic cancellation process when the dimensions considered are of order of magnitude. 
%
%To avoid the precision problems leading, we traverse our dynamic scene graph in a different way than the commonly used way for a scene graph, trying to minimizing the number of transformations executed.
%
%The closest (in an euclidean space) node from the virtual camera, the node which virtual camera is attached, is the current center of the coordinates of the system for the whole scene (center of the world coordinates) and will be called from now on the current center node.
%In our example in Figure~\ref{fig:scenegraph} this node is the item 4 node (Moon).
%Thus, instead of applying all translations from the root node to item 4 node, we only apply the transformations present in item 4 node in order to render its entity, minimizing the chances of a catastrophic cancellation happens.
%
%When visiting other item nodes, for instance, the item 5 node, we initially find the closest common ancestor node between the current center node and the item 5 node, i.e. the item 2 node.
%Instead of applying all transformations from the root node to item 5 node, we only apply the transformations from item 2 node to item 5 node (drawn as a blue arrow in Figure~\ref{fig:scenegraph}, and to obtain the final translation from the current center node to item 5 node, we subtract the translation from the item 2 node to the current center node (drawn as a red arrow in Figure~\ref{fig:scenegraph}).
%This procedure is equivalent to summing the vector $\mathbf{v}_1$ from item 2 node to item 5 node to vector $\mathbf{v}_2$ from item 5 node to item 2 node. See Figure~\ref{fig:vectorsum}
%
%The same idea can be extended for the other nodes.
%For the item 3 node, its position related to the current center node can be obtained adding the vectors $\mathbf{v}_2$, $\mathbf{v}_3$ and $\mathbf{v}_4$ in Figure~\ref{fig:vectorsum}.
%
%\joncomment{Emil, What do you want to talk about the perspective effects here?.}
%
%We use the perspective matrix:
%
%\begin{equation} \label{eq:ourperspectiveprojection}
%P = \mleft(
%\begin{array}{cccc}
%  \arctan(\frac{\theta_{h}}{2}) & 0 & 0 & 0\\
%  0 & \arctan(\frac{\theta_{v}}{2}) & 0 & 0\\
%  0 & 0 & 0 & 0\\
%  0 & 0 & -1 & 0
%\end{array}\mright)
%\end{equation}
%
%We manually write the gl\_FragDepth to a floating point buffer using the distance to the fragment computed with the pythagorean theorem.
%
%Clipping is performed for all content with negative w.
%
%\subsubsection{Dynamic camera attachment}
%
%The virtual camera in our system can move freely within 5 degrees of freedom (DOF).
%Considering only 3 DOF (3-d space movement), the camera can be close to the position of any node in our dynamic scene graph.
%Which means that once the camera is close to a node's position, this node must be the new current center node.
%This propriety of changing the current center node, and by consequence the scene's center of coordinate, based on the camera position and other nodes proximity is one of the factors contributing to the ``dynamic'' aspect of our scene graph.
%
%In order to determine which node should be the new current center node and attach the virtual camera to it, every time the camera is translated, its current position is tested against the node radius of the nodes in the dynamic scene graph.
%If the camera is inside a node's node radius, the camera position is tested against its children nodes' node radius (if present) to determine which new node should be the next current center node.
%
%%This idea is represented by the algorithm~\ref{alg:dynamicCamera}.
%
%%\begin{algorithm}\label{alg:dynamicCamera}
%% \caption{Dynamic Camera Attachment Algorithm}
%% \begin{algorithmic}[1]
%% \renewcommand{\algorithmicrequire}{\textbf{Input:}}
%% \renewcommand{\algorithmicensure}{\textbf{Output:}}
%% \REQUIRE in
%% \ENSURE  out
%% \\ \textit{Initialization} :
%%  \STATE first statement
%% \\ \textit{LOOP Process}
%%  \FOR {$i = l-2$ to $0$}
%%  \STATE statements..
%%  \IF {($i \ne 0$)}
%%  \STATE statement..
%%  \ENDIF
%%  \ENDFOR
%% \RETURN $P$
%% \end{algorithmic}
%%\end{algorithm}  
%
%\subsubsection{Dynamic object attachment}
%Not all nodes with a renderable entity in the dynamic scene graph are celestial bodies with known orbits. 
%One possible renderable entity is a mission space probe. 
%
%Space probes like New Horizons~\cite{stern2009new} travels through the solar system capturing important scientific data that is later sent to Earth.
%Because of New Horizons' trajectory is given by a path from Earth to Pluto, a commonly used scene graph would set New Horizon's node as a child of an existent node in the tree (Earth's node, e.g.) and when the New Horizons probe is rendered close to Pluto, the transformations necessary to render that entity will lead to a precision problem.
%Equivalently, setting Pluto's node as the parent of New Horizons' node will generate precision problems when rendering New Horizons probe close to Earth.
%
%The solution to this problem is to have a dynamic node attachment, i.e., during the traveling of the New Horizons probe its node changes the parent relationship with the other nodes in the dynamic scene graph.
%This parent changing (adoption), is done based on checking the moving node's position against the other node's node radius.
%If the moving node position is inside a node radius, the children of this node are also tested against to determine the closest node which the moving node is inside the node radius.
%Once this closes node is determined, the moving node is adopted by this new node and the transformations adjusted accordingly.
%
%%The algorithm~\ref{alg:dynamicAdoption} represents this idea. \joncomment{Improve this paragraph.}
%%
%%\begin{algorithm}\label{alg:dynamicAdoption}
%% \caption{Dynamic Node Attachment Algorithm - Node Adoption}
%% \begin{algorithmic}[1]
%% \renewcommand{\algorithmicrequire}{\textbf{Input:}}
%% \renewcommand{\algorithmicensure}{\textbf{Output:}}
%% \REQUIRE in
%% \ENSURE  out
%% \\ \textit{Initialization} :
%%  \STATE first statement
%% \\ \textit{LOOP Process}
%%  \FOR {$i = l-2$ to $0$}
%%  \STATE statements..
%%  \IF {($i \ne 0$)}
%%  \STATE statement..
%%  \ENDIF
%%  \ENDFOR
%% \RETURN $P$
%% \end{algorithmic}
%%\end{algorithm}  
%
%\subsubsection{Model view matrix computation}
%
%How to calculate the final Model View Matrix...
%
\subsection{Representation of depth} \label{sec:method:ds}
In our approach, we use a modified perspective matrix with $\lambda_1 = \lambda_2 = 0$, yielding $\mathbf{z}_c = 0$ for all vertices, thus disabling the near and far planes.
Geometry behind the camera is discarded due to the criterion of $-\mathbf{c}_w < \mathbf{c}_w$ imposed by the OpenGL pipeline.
For depth comparisons, we use 32-bit floating point numbers representing the distance to the fragment being rendered.

To avoid exponent overflow and underflow for distant and close fragments, we first divide the view coordinate vector $v$ by its component with largest magnitude $\mathbf{v}_{\text{max}}$, perform the computation using the pythagoran theorem, and multiply by $\mathbf{v}_{\text{max}}$.

Depth sorting is performed using conventional z-buffering with a 32-bit floating point buffer. For scenes with volumetric content or transparent geometry, we use the A-buffer technique\cite{lindholm2015hybrid} for order-independent transparency. When using ray casting for volumetric rendering, the distance taken along the ray can be compared with the distance of rendered geometry in order to perform early ray termination.

By always rendering objects in their physical location, as opposed to the reprojection of objects performed in the ScaleGraph approach, our method transparently handles stereoscopic rendering.

%\subsection{Stereoscopic Rendering}


\section{Results} \label{sec:result}
We applied our framwork to a variety of use cases, most of which have been shown to audiences at public events.
In this section, we shortly describe these use-cases and how our proposed frameworks was used in four categories.

\noindent \textbf{Space craft. }
As one of the major motivations for developing this framework, spacecraft are a inherently small objects that need to be visualized precisely with respect to a an object they are inspecting.
One example is shown in Figure~\ref{fig:teaser} where New Horizons space craft is shown at 80\,Mm distance to Pluto and around $6 \cdot 10^{12}$\,m from the Sun.
As described in Section~\ref{sec:theory:pl}, floating point numbers only have a precision of about 300\,km at the distance of Pluto, which would make a traditional approach infeasible.
A second space craft is ESA's Rosetta shown in Figure~\ref{fig:res:stereo} as a stereoscopic rendering.

\noindent \textbf{Navigation. }
Figure~\ref{fig:imagesequence} shows individual frames of a seamless nagivation from the surface of Earth, where the mountains of the Alps are rendered with their correct height and surface textures, out to the surface of Titan orbiting Saturn, showing color images of the moon's surface.
At no point during this $1.65$\, billion km user controlled journey artifacts are visible.

\noindent \textbf{Virtual globes. }
High resolution surface features are an additional challenge when representing an entire solar system in a single scene graph.
Individual surface features are smaller than the available floating point precision.
Figure~\ref{fig:res:pluto} shows an accurate rendering of the latest composite images and height maps on Pluto~\cite{stern2015pluto} with a resolution of between 2 and 5\,km/pixel.
Figure~\ref{fig:res:mars} shows detailed structures on the surface of Mars as returned from the Mars Reconnaissance Orbiter's HiRISE camera~\cite{mcewen2007mars}.
It shows details with a resolution of 25\,cm per pixel.

\noindent \textbf{Volumetric data. }
As described in Section~\ref{sec:method:ds}, our system allows for the seamless integration of volumetric data.
Figure~\ref{fig:res:galaxy} shows a frame of a three-channel time-varying volumetric rendering of the Milky Way~\cite{XXX}\alexcomment{Waiting for a citation from Vivian}.
With this scene, the root node of the scene graph is the center of the Milky Way and the solar system and all its contents are at a distance of $2,5 \cdot 10^{20}$ from the center with the volume's bounding box of $\approx 10^{21} \times 10^{21} \times 1.5 \cdot 10^{19}$\,m.

\begin{figure}
\centering
\fbox{\includegraphics[width=\linewidth]{figure/pluto.png}}
\caption{A rendering of Pluto's surface with a height field reconstructed from New Horizons' images showing the Zheng He Montes and al-Idrisi Montes in front of Tombaugh Regio.}
\label{fig:res:pluto}
\end{figure}

\begin{figure}
\centering
\fbox{\includegraphics[width=\linewidth]{figure/mars.png}}
\caption{Details of structures with up to 25\,cm per pixel resolution in a rendering of the surface features in West Candor Chasma on Mars as captured from the HiRISE camera.
%, showing details of structures with up to 25\,cm per pixel in resolution.}
}
\label{fig:res:mars}
\end{figure}

\begin{figure}
\centering
\fbox{\includegraphics[width=\linewidth]{figure/stereo.png}}
\caption{Our approach supports stereoscopic rendering (here, amber/blue anaglyph) exemplified with the Rosetta/Philae separation for the landing on the comet 67P/Churyumov-Gerasimenko.}
\label{fig:res:stereo}
\end{figure}

\begin{figure}
\centering
\fbox{\includegraphics[width=\linewidth]{figure/galaxy.png}}
\caption{A volumetric rendering of a three-channel simulation of the Milky Way with a bounding box size in the order of $10^{20}$\,m.}
\label{fig:res:galaxy}
\end{figure}



%-------------------------------------------------------------------------
\section{Conclusions}
In this work, we presented a framework that supports high-precision transformations in a scene graph by dynamically attaching the camera to the closest node and using this node as an origin for the scene traversal.
The dynamic scene graph enables a higher level of individual precision than which is otherwise possible.
By supporting efficient dynamic reattachment of the camera to nodes, it becomes possible to navigate a scene encompassing scales and distances much larger than floating point precision would normally permit.
The same reattachment technique can be applied to other scene graph nodes to provide the ability for scene graph nodes to retain high precision at multiple locations, for example, a space craft that requires high precision at multiple planets as it moves through space.
The scene graph traversal uses the shortest path to every other scene graph node and thus circumvents potentially large translation values that would otherwise lead to precision degradation during the rendering.

We implemented the framework in the open-source software platform OpenSpace which has the goal of being able to visualize and contextualize a wide range of astrophysical data.
The framework was successfully tested on the visualization of multiple space craft missions~\cite{Bock_2015}, on virtual globes, and volumetric rendering.
The framework is flexible enough, however, to be implemented and used in any generic scene graph implementation.

\subsection{Future Work}
For future work, we would like to include higher-level wayfinding techniques such as the ones explored in Li~\etal~\cite{li2006scalable} and generalize these to both macro and microcosmos scenarios. Furthermore, a more rigorous automatic determination of scene node extents, based on analysis of an object's movement, would be beneficial for displaying objects which are not moving on regular orbits.

\section{Acknowledgments}
We would like to acknowledge the Swedish e-Science Research Center (SeRC) for their support of this work. Parts of this work were supported by NASA under award No NNX16AB93A. Additional thanks to Karl Bladin and Erik Broberg for their work on the planetary renderer and ESRI for providing the Earth data.


%-------------------------------------------------------------------------

\newpage

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{dsg-bib}

%-------------------------------------------------------------------------

\end{document}
