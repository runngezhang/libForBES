% Copyright (C) 2015, Lorenzo Stella and Panagiotis Patrinos
%
% This file is part of ForBES.
% 
% ForBES is free software: you can redistribute it and/or modify
% it under the terms of the GNU Lesser General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% ForBES is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
% GNU Lesser General Public License for more details.
% 
% You should have received a copy of the GNU Lesser General Public License
% along with ForBES. If not, see <http://www.gnu.org/licenses/>.

function obj = distBox(lb,ub,weights)
    % Proximal mapping for (weighted) distance from a box [lb,ub]
    obj.makeprox = @(gam0) @(x, gam) call_distBox_prox(x, gam, lb, ub, weights);
end

function [prox, val] = call_distBox_prox(x, gam, lb, ub, weights)
    mu = gam*weights;
    prox = max(x - ub - mu,0) - max(lb - x - mu,0) + min(max(x,lb),ub);
    % prox = x - min(max(x,lb - mu),ub + mu)+ min(max(x,lb),ub);%
    if nargout>1
        finw = ~isinf(weights);    
        val = sum(weights(finw).*abs(prox(finw)-min(max(prox(finw),lb(finw)),ub(finw))));
    end
end
